#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <cassert>
#include <string>

#include <boost/iostreams/filter/gzip.hpp>

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
        io::filtering_streambuf<io::input> in;
        in.push(io::gzip_compressor());
        in.push(ifile);
        fs::path encrypted_path = fs::path(path) += ".x";
        std::ofstream ofile(encrypted_path.c_str(), ofile.binary);
        if (!ofile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }
        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void decrypt_file(fs::path const& path) {
        assert(fs::exists(path));
        std::ifstream ifile(path.c_str(), ifile.binary);
        if (!ifile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }
        io::filtering_streambuf<io::input> in;
        in.push(io::gzip_decompressor());
        in.push(ifile);
        fs::path decrypted_path = fs::path(path);
        if (decrypted_path.extension() == ".x") {
            decrypted_path.replace_extension();
        }
        std::ofstream ofile(decrypted_path.c_str(), ofile.binary);
        if (!ofile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }
        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void encrypt_directory(fs::path const& path) {
    }

    void decrypt_directory(fs::path const& path) {
    }

    void wipe_file(fs::path const& path, bool debug) {
        assert(fs::exists(path));
        std::ofstream ofile(path.c_str(), ofile.binary | ofile.in | ofile.ate);
        if (!ofile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }
        auto pos = ofile.tellp();
        assert(pos != -1);
        ofile.seekp(0, std::ios_base::beg);
        auto off = std::streamoff(pos);
        while (off > 0) {
            ofile << std::ofstream::char_type(0);
            --off;
        }
        ofile.close();
        if (debug == false) {
            fs::remove(path);
        }
    }

    void wipe_directory(fs::path const& path, bool debug) {
    }
}
