#ifndef CRYPTOR_HPP
#define CRYPTOR_HPP
#pragma once

#include <boost/filesystem.hpp>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    void encrypt(           fs::path const& path, fs::path const& output_dir, bool test);
    void decrypt(           fs::path const& path, fs::path const& output_dir, bool test);
    void wipe(              fs::path const& path,                             bool test);

    void encrypt_file(      fs::path const& path, fs::path const& dest);
    void encrypt_directory( fs::path const& path, fs::path const& dest);
    void decrypt_file(      fs::path const& path, fs::path const& dest);
    void decrypt_directory( fs::path const& path, fs::path const& dest);
    void wipe_file(         fs::path const& path);
    void wipe_directory(    fs::path const& path);
}

#endif
