#include "cipher.hpp"
#include <boost/filesystem.hpp>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <cassert>
#include <memory>

#include <iostream>
#include <openssl/bio.h>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    namespace {
        struct aes_gcm_header_t {
            unsigned char gcm_key[256 / 8], gcm_iv [128 / 8];
            unsigned char gcm_tag[128 / 8];
        } __attribute__((packed));

        void check_params(fs::path const& source,
                          fs::path const& output_dir) {

            if (fs::exists(source) == false) {
                throw std::string(source.c_str()) +=
                    " doesn't exist";
            }

            if (fs::is_regular(source) == false) {
                throw std::string(source.c_str()) +=
                    " is not regular file";
            }

            if (fs::exists(output_dir) == false) {
                throw std::string(output_dir.c_str()) +=
                    " doesn't exist";
            }

            if (fs::is_directory(output_dir) == false) {
                throw std::string(output_dir.c_str()) +=
                    " is not directory";
            }
        }
    }

    Cipher_aes_gcm::Cipher_aes_gcm(std::string const& password)
        : password_(password) {
    }

    Cipher_aes_gcm::~Cipher_aes_gcm() = default;

    void Cipher_aes_gcm::encrypt(fs::path const& source,
                                 fs::path const& output_dir) {

        check_params(source, output_dir);

        std::ifstream ifile(source.c_str(), ifile.binary);
        if (!ifile) {
            throw std::string(source.c_str()) +=
                " can't be open";
        }

        auto destination = fs::path(source) += ".x";
        std::ofstream ofile(destination.c_str(), ofile.binary | ofile.trunc);
        if (!ofile) {
            throw std::string(destination.c_str()) +=
                " can't be encrypted. Error with a destination file.";
        }

        aes_gcm_header_t header = {};
        if (1 != RAND_bytes((unsigned char*)&header, 
            sizeof(header) - sizeof(header.gcm_tag))) {
            throw "unknown error: RAND_bytes";
        }

        if (!ofile.write((char *)&header, std::streamsize(sizeof(header)))) {
            throw "unknown error: ofile.write";
        }

        auto ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw "unknown error: EVP_CIPHER_CTX_new";
        }

        try {
            if (1 != EVP_EncryptInit_ex(
                ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
                throw "unknown error: EVP_EncryptInit_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(header.gcm_iv), nullptr)) {
                throw "unknown error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_IVLEN";
            }

            if (1 != EVP_EncryptInit_ex(
                ctx, nullptr, nullptr, header.gcm_key, header.gcm_iv)) {
                throw "unknown error: EVP_EncryptInit_ex";
            }

            unsigned char pt_buffer[1024];
            unsigned char ct_buffer[1024];
            int pt_len = 0;
            int ct_len = 0;

            do {
                ifile.read((char *)pt_buffer, sizeof(pt_buffer));
                pt_len = int(ifile.gcount());
                if (pt_len > 0) {
                    if (1 != EVP_EncryptUpdate(
                        ctx, ct_buffer, &ct_len, pt_buffer, pt_len)) {
                        throw "unknown error: EVP_EncryptUpdate";
                    }

                    if (!ofile.write((char *)ct_buffer, std::streamsize(ct_len))) {
                        throw "unknown error: ofile.write";
                    }
                }

            } while(ifile);

            if (1 != EVP_EncryptFinal_ex(ctx, ct_buffer, &ct_len)) {
                throw "unknown error: EVP_EncryptFinal_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_GET_TAG, sizeof(header.gcm_tag), header.gcm_tag)) {
                throw "unknown error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_GET_TAG";
            }

            ofile.seekp(sizeof(header) - sizeof(header.gcm_tag), ofile.beg);
            if (!ofile.write((char *)header.gcm_tag,
                             std::streamsize(sizeof(header.gcm_tag)))) {
                throw "unknown error: ofile.write";
            }

            throw (const char*)nullptr;
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

    void Cipher_aes_gcm::decrypt(fs::path const& source,
                                 fs::path const& output_dir) {

        check_params(source, output_dir);

        std::ifstream ifile(source.c_str(), ifile.binary);
        if (!ifile) {
            throw std::string(source.c_str()) +=
                " can't be open";
        }

        auto destination = fs::path(source);
        if (destination.extension() == ".x") {
            destination.replace_extension();
        }

        std::ofstream ofile(destination.c_str(), ofile.binary | ofile.trunc);
        if (!ofile) {
            throw std::string(destination.c_str()) +=
                " can't be encrypted. Error with a destination file.";
        }

        aes_gcm_header_t header = {};
        ifile.read((char *)&header, sizeof(header));
        if (ifile.gcount() != sizeof(header)) {
            throw "unknown error: ifile.read";
        }

        auto ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw "unknown error: EVP_CIPHER_CTX_new";
        }

        try {
            if (1 != EVP_DecryptInit_ex(
                ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
                throw "unknown error: EVP_DecryptInit_ex";
            }

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(header.gcm_iv), nullptr)) {
                throw "unknown error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_IVLEN";
            }

            if (1 != EVP_DecryptInit_ex(
                ctx, nullptr, nullptr, header.gcm_key, header.gcm_iv)) {
                throw "unknown error: EVP_DecryptInit_ex";
            }

            unsigned char pt_buffer[1024];
            unsigned char ct_buffer[1024];
            int pt_len = 0;
            int ct_len = 0;

            do {
                ifile.read((char *)ct_buffer, sizeof(ct_buffer));
                ct_len = int(ifile.gcount());
                if (ct_len > 0) {
                    if (1 != EVP_DecryptUpdate(
                        ctx, pt_buffer, &pt_len, ct_buffer, ct_len)) {
                        throw "unknown error: EVP_DecryptUpdate";
                    }

                    if (!ofile.write((char *)pt_buffer, std::streamsize(pt_len))) {
                        throw std::string(destination.c_str()) +=
                            " : error writing";
                    }
                }

            } while(ifile);

            if (1 != EVP_CIPHER_CTX_ctrl(
                ctx, EVP_CTRL_AEAD_SET_TAG, sizeof(header.gcm_tag), header.gcm_tag)) {
                throw "unknown error: EVP_CIPHER_CTX_ctrl - EVP_CTRL_AEAD_SET_TAG";
            }

            if (1 != EVP_EncryptFinal_ex(ctx, ct_buffer, &ct_len)) {
                throw "unknown error: EVP_DecryptFinal_ex";
            }

            throw (const char*)nullptr;
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
}
