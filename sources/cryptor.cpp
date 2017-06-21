#include "cryptor.hpp"
#include <boost/filesystem.hpp>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    void encrypt(
        fs::path const& path, fs::path const& output_dir, bool test) {
        if (!fs::exists(path)) {
            throw std::string(path.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_regular(path)) {

            fs::path dest = fs::path(path) += ".x";
            encrypt_file(path, dest);

        } else if (fs::is_directory(path)) {

        } else {
            throw std::string(path.c_str()) +=
                " has unsupported file type";
        }
    }

    void decrypt(
        fs::path const& path, fs::path const& output_dir, bool test) {
        if (!fs::exists(path)) {
            throw std::string(path.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_regular(path)) {

            fs::path dest = fs::path(path);
            if (dest.extension() == ".x") {
                dest.replace_extension();
            }

            decrypt_file(path, dest);

        } else if (fs::is_directory(path)) {

        } else {
            throw std::string(path.c_str()) +=
                " has unsupported file type";
        }
    }

    void wipe(fs::path const& path, bool test) {
        if (!fs::exists(path)) {
            throw std::string(path.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_regular(path)) wipe_file(path); else
        if (fs::is_directory(path)) wipe_directory(path); else {
            throw std::string(path.c_str()) +=
                " has unsupported file type";
        }

        if (test == false) fs::remove_all(path);
    }
}
