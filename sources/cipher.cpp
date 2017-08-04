#include "cipher.hpp"
#include <boost/filesystem.hpp>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/blowfish.h>
#include <cassert>
#include <memory>

namespace ns_cipher {
    namespace fs = boost::filesystem;
    namespace {
        struct aes_gcm_header_t {
            unsigned char gcm_key[256 / 8], gcm_iv[128 / 8], gcm_tag[128 / 8];

        } __attribute__((packed));

        void check_params(fs::path const& source,
                          fs::path const& output_dir) {

            if (!fs::exists(source)) {
                throw std::move(std::string(source.c_str()) +=
                    " doesn't exist");
            }

            if (fs::exists(output_dir)) {
                if (!fs::is_directory(output_dir)) {
                    throw std::move(std::string(output_dir.c_str()) +=
                        " is not a directory");
                }
            }
        }
    }

    Cipher_aes_gcm::Cipher_aes_gcm(std::string const& password): bf_key_{} {
        unsigned char iv[8] = {};
        BF_set_key(&bf_key_, password.size(), reinterpret_cast<unsigned char const*>(password.c_str()));
    }

    void Cipher_aes_gcm::encrypt_file(fs::path const& source,
                                      fs::path const& output_dir) const {

        if (!fs::exists(output_dir)) {
            if (!fs::create_directories(output_dir)) {
                throw std::move(std::string(output_dir.c_str()) +=
                    " can't be created");
            }
        }

        std::ifstream ifile(source.c_str(), ifile.binary);
        if (!ifile) {
            throw std::move(std::string(source.c_str()) +=
                " can't be open");
        }

        auto destination = (fs::path(output_dir) /= source.filename()) += ".x";
        std::ofstream ofile(destination.c_str(), ofile.binary | ofile.trunc);
        if (!ofile) {
            throw std::move(std::string(destination.c_str()) +=
                " can't be encrypted. Error with a destination file.");
        }

        aes_gcm_header_t header = {};
        if (!ofile.write(reinterpret_cast<char *>(&header), std::streamsize(sizeof(header)))) {
            throw "error: ofile.write";
        }

        if (1 != RAND_bytes(reinterpret_cast<unsigned char*>(&header), 
            sizeof(header) - sizeof(header.gcm_tag))) {
            throw "error: RAND_bytes";
        }

        auto ctx = EVP_CIPHER_CTX_new();
        if (nullptr == ctx) {
            throw "error: EVP_CIPHER_CTX_new";
        }

        try {
            if (1 != EVP_EncryptInit_ex(
                ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
                throw "error: EVP_EncryptInit_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(header.gcm_iv), nullptr)) {
                throw "error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_IVLEN";
            }

            if (1 != EVP_EncryptInit_ex(
                ctx, nullptr, nullptr, header.gcm_key, header.gcm_iv)) {
                throw "error: EVP_EncryptInit_ex";
            }

            unsigned char pt_buffer[1024];
            unsigned char ct_buffer[1024];
            int pt_len = 0;
            int ct_len = 0;

            do {
                ifile.read(reinterpret_cast<char *>(pt_buffer), sizeof(pt_buffer));
                pt_len = int(ifile.gcount());
                if (pt_len > 0) {
                    if (1 != EVP_EncryptUpdate(
                        ctx, ct_buffer, &ct_len, pt_buffer, pt_len)) {
                        throw "error: EVP_EncryptUpdate";
                    }

                    if (!ofile.write(reinterpret_cast<char *>(ct_buffer), std::streamsize(ct_len))) {
                        throw "error: ofile.write";
                    }
                }

            } while(ifile);

            if (1 != EVP_EncryptFinal_ex(ctx, ct_buffer, &ct_len)) {
                throw "error: EVP_EncryptFinal_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_GET_TAG, sizeof(header.gcm_tag), header.gcm_tag)) {
                throw "error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_GET_TAG";
            }

            aes_gcm_header_t blowfish_header = {};
            unsigned char iv[8] = {};
            BF_cbc_encrypt(reinterpret_cast<unsigned char*>(&header), reinterpret_cast<unsigned char*>(&blowfish_header),
                long(sizeof(aes_gcm_header_t)), &bf_key_, iv, BF_ENCRYPT);

            ofile.seekp(0, ofile.beg);
            if (!ofile.write(
                reinterpret_cast<char *>(&blowfish_header), std::streamsize(sizeof(blowfish_header)))) {
                throw "error: ofile.write";
            }

            throw static_cast<const char*>(nullptr);
        } catch (const char *error_msg) {

            EVP_CIPHER_CTX_free(ctx);
            ifile.close();
            ofile.close();

            if (error_msg != nullptr) {
                fs::remove(destination);
                throw;
            }
        }
    }

    void Cipher_aes_gcm::decrypt_file(fs::path const& source,
                                      fs::path const& output_dir) const {

        if (!fs::exists(output_dir)) {
            if (!fs::create_directories(output_dir)) {
                throw std::move(std::string(output_dir.c_str()) +=
                    " can't be created");
            }
        }

        std::ifstream ifile(source.c_str(), ifile.binary);
        if (!ifile) {
            throw std::move(std::string(source.c_str()) +=
                " can't be open");
        }

        fs::path destination = fs::path(output_dir) /= source.filename();
        if (destination.extension() == ".x") {
            destination.replace_extension();
        }

        std::ofstream ofile(destination.c_str(), ofile.binary | ofile.trunc);
        if (!ofile) {
            throw std::move(std::string(destination.c_str()) +=
                " can't be encrypted. Error with a destination file.");
        }

        aes_gcm_header_t blowfish_header = {};
        ifile.read(reinterpret_cast<char *>(&blowfish_header), sizeof(blowfish_header));
        if (ifile.gcount() != sizeof(blowfish_header)) {
            throw "error: ifile.read";
        }

        aes_gcm_header_t header = {};
        unsigned char iv[8] = {};
        BF_cbc_encrypt(reinterpret_cast<unsigned char const*>(&blowfish_header), reinterpret_cast<unsigned char*>(&header),
            sizeof(aes_gcm_header_t), &bf_key_, iv, BF_DECRYPT);

        auto ctx = EVP_CIPHER_CTX_new();
        if (nullptr == ctx) {
            throw "error: EVP_CIPHER_CTX_new";
        }

        try {
            if (1 != EVP_DecryptInit_ex(
                ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
                throw "error: EVP_DecryptInit_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(header.gcm_iv), nullptr)) {
                throw "error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_IVLEN";
            }

            if (1 != EVP_DecryptInit_ex(
                ctx, nullptr, nullptr, header.gcm_key, header.gcm_iv)) {
                throw "error: EVP_DecryptInit_ex";
            }

            unsigned char pt_buffer[1024];
            unsigned char ct_buffer[1024];
            int pt_len = 0;
            int ct_len = 0;

            do {
                ifile.read(reinterpret_cast<char *>(ct_buffer), sizeof(ct_buffer));
                ct_len = int(ifile.gcount());
                if (ct_len > 0) {
                    if (1 != EVP_DecryptUpdate(
                        ctx, pt_buffer, &pt_len, ct_buffer, ct_len)) {
                        throw "error: EVP_DecryptUpdate";
                    }

                    if (!ofile.write(reinterpret_cast<char *>(pt_buffer), std::streamsize(pt_len))) {
                        throw "error: ofile.write";
                    }
                }

            } while(ifile);

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_TAG, sizeof(header.gcm_tag), header.gcm_tag)) {
                throw "error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_TAG";
            }

            if (1 != EVP_EncryptFinal_ex(ctx, ct_buffer, &ct_len)) {
                throw "error: EVP_DecryptFinal_ex";
            }

            throw static_cast<const char*>(nullptr);
        } catch (const char *error_msg) {

            EVP_CIPHER_CTX_free(ctx);
            ifile.close();
            ofile.close();

            if (error_msg != nullptr) {
                fs::remove(destination);
                throw;
            }
        }
    }

    void Cipher_aes_gcm::encrypt_directory(fs::path const& source,
                                           fs::path const& output_dir) const {

        auto prefix_size = source.has_parent_path() ?
            source.parent_path().size() + 1 : 0;

        fs::recursive_directory_iterator i(source);
        fs::recursive_directory_iterator e;
        for(; i != e; ++i) {
            auto curr_path = i->path();
            if (".DS_Store" == curr_path.filename()) continue;

            if (fs::is_regular(curr_path)) {
                auto dest_dir = (fs::path(output_dir) /= (
                    std::string(curr_path.c_str()).substr(
                        prefix_size))).parent_path();
                encrypt_file(curr_path, dest_dir);
            }
        }
    }

    void Cipher_aes_gcm::decrypt_directory(fs::path const& source,
                                           fs::path const& output_dir) const {

        auto prefix_size = source.has_parent_path() ?
            source.parent_path().size() + 1 : 0;

        fs::recursive_directory_iterator i(source);
        fs::recursive_directory_iterator e;
        for(; i != e; ++i) {
            auto curr_path = i->path();
            if (".DS_Store" == curr_path.filename()) continue;

            if (fs::is_regular(curr_path)) {
                auto dest_dir = (fs::path(output_dir) /= (
                    std::string(curr_path.c_str()).substr(
                        prefix_size))).parent_path();
                decrypt_file(curr_path, dest_dir);
            }
        }
    }

    void Cipher_aes_gcm::encrypt(fs::path const& source,
                                 fs::path const& output_dir) const {

        check_params(source, output_dir);

        if (fs::is_regular(source)) {
            encrypt_file(source, output_dir);

        } else if (fs::is_directory(source)) {
            encrypt_directory(source, output_dir);

        } else {
            throw std::move(std::string(source.c_str()) +=
                " has unsupported file type");
        }
    }

    void Cipher_aes_gcm::decrypt(fs::path const& source,
                                 fs::path const& output_dir) const {

        check_params(source, output_dir);

        if (fs::is_regular(source)) {
            decrypt_file(source, output_dir);

        } else if (fs::is_directory(source)) {
            decrypt_directory(source, output_dir);

        } else {
            throw std::move(std::string(source.c_str()) +=
                " has unsupported file type");
        }
    }
}
