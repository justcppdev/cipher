#include "cryptor.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <cassert>
#include <string>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    void wipe_file(fs::path const& path) {
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
    }

    void wipe_directory(fs::path const& path) {
        assert(fs::exists(path));
        fs::directory_iterator i(path);
        fs::directory_iterator e;
        for(; i != e; ++i) {
            auto curr_path = i->path();

            if (fs::is_directory(curr_path)) {
                wipe_directory(curr_path);

            } else if (fs::is_regular(curr_path)) {
                wipe_file(curr_path);
            }
        }
    }
}
