#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <cassert>

#include <boost/iostreams/filter/gzip.hpp>
#include <iostream>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    namespace io = boost::iostreams;

    void encrypt_file(fs::path const& path) {
        assert(fs::exists(path));
        std::ifstream ifile(path.c_str(), ifile.binary);
        assert(ifile);
        io::filtering_streambuf<io::input> in;
        in.push(io::gzip_compressor()); // !!!
        in.push(ifile);
        fs::path encrypted_path = fs::path(path) += ".x";
        std::ofstream ofile(encrypted_path.c_str(), ofile.binary);
        assert(ofile);
        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void decrypt_file(fs::path const& path) {
        assert(fs::exists(path));
        std::ifstream ifile(path.c_str(), ifile.binary);
        assert(ifile);
        io::filtering_streambuf<io::input> in;
        in.push(io::gzip_decompressor()); // !!!
        in.push(ifile);
        fs::path decrypted_path = fs::path(path);
        if (decrypted_path.extension() == ".x") {
            decrypted_path.replace_extension();
        }
        std::ofstream ofile(decrypted_path.c_str(), ofile.binary);
        assert(ofile);
        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void encrypt_directory(fs::path const& path) {
    }

    void decrypt_directory(fs::path const& path) {
    }
}
