#ifndef CRYPTOR_HPP
#define CRYPTOR_HPP
#pragma once

#include <boost/filesystem.hpp>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    void encrypt_file(fs::path const& path);
    void decrypt_file(fs::path const& path);
    void encrypt_directory(fs::path const& path);
    void decrypt_directory(fs::path const& path);
}

#endif
