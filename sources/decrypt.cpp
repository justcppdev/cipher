#include "cryptor.hpp"
#include "decryptor.hpp"
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <cassert>
#include <string>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    namespace io = boost::iostreams;

    void decrypt_file(fs::path const& path) {

        assert(fs::exists(path));
        std::ifstream ifile(path.c_str(), ifile.binary);
        if (!ifile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }

        Decryptor decryptor(path);
        auto dest = decryptor.destination();

        io::filtering_streambuf<io::input> in;
        in.push(decryptor);
        in.push(ifile);

        std::ofstream ofile(dest.c_str(), ofile.binary);
        if (!ofile) {
            throw std::string(path.c_str()) +=
                " can't be open";
        }

        io::copy(in, ofile);
        ifile.close();
        ofile.close();
    }

    void decrypt_directory(fs::path const& path) {
    }

    void decrypt(fs::path const& path, fs::path const& output_dir, bool test) {
        if (!fs::exists(path)) {
            throw std::string(path.c_str()) +=
                " doesn't exist";
        }

        if (fs::is_regular(path)) {
            decrypt_file(path);

        } else if (fs::is_directory(path)) {

        } else {
            throw std::string(path.c_str()) +=
                " has unsupported file type";
        }
    }
}
