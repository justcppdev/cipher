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
        virtual void encrypt(fs::path const& source, fs::path const& output_dir) const = 0;
        virtual void decrypt(fs::path const& source, fs::path const& output_dir) const = 0;

        virtual ~Cipher() = default;
    };

    class Cipher_aes_gcm final: public Cipher {
    public:
        explicit Cipher_aes_gcm(std::string const& password);

        void encrypt          (fs::path const& source, fs::path const& output_dir) const override;
        void decrypt          (fs::path const& source, fs::path const& output_dir) const override;
    private:
        void encrypt_file     (fs::path const& source, fs::path const& output_dir) const;
        void decrypt_file     (fs::path const& source, fs::path const& output_dir) const;
        void encrypt_directory(fs::path const& source, fs::path const& output_dir) const;
        void decrypt_directory(fs::path const& source, fs::path const& output_dir) const;

        BF_KEY bf_key_;
    };
}

#endif
