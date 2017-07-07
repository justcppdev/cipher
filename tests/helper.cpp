#include "helper.hpp"
#include <boost/filesystem.hpp>
#include <catch.hpp>
#include <fstream>
#include <cstdlib>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    fs::path tmp_file(fs::path const& dir) {
        REQUIRE(fs::exists(dir));
        fs::path path = fs::path(dir) /= fs::unique_path();
        std::ofstream ofile(path.c_str());
        REQUIRE(ofile);
        ofile << path;
        ofile.close();
        return path;
    }

    fs::path tmp_dir(fs::path const& dir) {
        REQUIRE(fs::exists(dir));
        auto tmp_path = fs::path(dir) /= fs::unique_path();
        REQUIRE(fs::create_directory(tmp_path));

        for(int i = 3; i > 0; --i) {
            auto path = fs::path(tmp_path) /= fs::unique_path();
            REQUIRE(fs::create_directory(path));
            for(int j = 5; j > 0; --j) {
                tmp_file(path);
            }
            tmp_file(tmp_path);
        }

        return tmp_path;
    }

    namespace {
        bool is_wiped_file(fs::path const& path) {
            std::ifstream ifile(path.c_str(), ifile.binary);
            REQUIRE(ifile);
            bool result = true;
            char c;
            while(!ifile.eof()) {
                ifile >> c;
                if (c != 0) {
                    result = false;
                    break;
                }
            }
            ifile.close();
            return result;
        } 

        bool is_wiped_directory(fs::path const& path) {
            fs::recursive_directory_iterator i(path);
            fs::recursive_directory_iterator e;
            for(; i != e; ++i) {
                fs::path curr_path = i->path();
                if (fs::is_regular(curr_path)) {
                    if (is_wiped_file(curr_path) == false)
                        return false;
                }
            }
            return true;
        }
    }

    bool is_wiped(fs::path const& path) {
        REQUIRE(fs::exists(path));

        if (fs::is_regular(path)) {
            return is_wiped_file(path);

        } else if (fs::is_directory(path)) {
            return is_wiped_directory(path);

        } else {
            return false;
        }
    }
}
