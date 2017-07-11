#ifndef CIPHER_HPP
#define CIPHER_HPP
#pragma once

#include <boost/filesystem.hpp>
#include <openssl/blowfish.h>
#include <string>

namespace ns_cipher {
    namespace fs = boost::filesystem;
    class Cipher {
    public:
        virtual void encrypt(fs::path const& source, fs::path const& output_dir) = 0;
        virtual void decrypt(fs::path const& source, fs::path const& output_dir) = 0;

        virtual ~Cipher() = default;
    };

    class Cipher_aes_gcm final: public Cipher {
    public:
        Cipher_aes_gcm(std::string const& password);
       ~Cipher_aes_gcm();

        void encrypt          (fs::path const& source, fs::path const& output_dir) override;
        void decrypt          (fs::path const& source, fs::path const& output_dir) override;
    private:
        void encrypt_file     (fs::path const& source, fs::path const& output_dir);
        void decrypt_file     (fs::path const& source, fs::path const& output_dir);
        void encrypt_directory(fs::path const& source, fs::path const& output_dir);
        void decrypt_directory(fs::path const& source, fs::path const& output_dir);

        BF_KEY bf_key_;
    };
}

#endif
