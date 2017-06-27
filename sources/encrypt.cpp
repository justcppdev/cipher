#include "cryptor.hpp"
#include "encryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <cassert>
#include <string>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    namespace io = boost::iostreams;

    void encrypt_file(fs::path const& path) {
        assert(fs::exists(path));
        std::ifstream ifile(path.c_str(), ifile.binary);
        if (!ifile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }

        std::string const passwd = "password";
        Encryptor encryptor(passwd);
        io::filtering_streambuf<io::input> in;
        in.push(encryptor);
        in.push(ifile);

        auto destination = fs::path(path) += ".x";
        std::ofstream ofile(destination.c_str(), ofile.binary | ofile.trunc);
        if (!ofile) {
            throw std::string(path.c_str()) +=
                " can't be encrypted. Error with a destination device.";
        }

        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void encrypt_directory(fs::path const& path) {
    }

    void encrypt(fs::path const& path, fs::path const& output_dir, bool test) {
        if (!fs::exists(path)) {
            throw std::string(path.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_regular(path)) {
            encrypt_file(path);

        } else if (fs::is_directory(path)) {

        } else {
            throw std::string(path.c_str()) +=
                " has unsupported file type";
        }
    }
}
