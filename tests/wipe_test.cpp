#include <catch.hpp>
#include "wipe.hpp"
#include "helper.hpp"
#include <boost/filesystem.hpp>
#include <memory>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    TEST_CASE("wipe", "[wipe]") {
        auto deleter = [](fs::path *p) {
            if (p != nullptr) {
                if (fs::exists(*p))
                    fs::remove_all(*p);
                delete p;
            }
        };

        std::unique_ptr<fs::path, decltype(deleter)>
            path(nullptr, deleter);

        SECTION("file") {
            REQUIRE_NOTHROW(path.reset(new fs::path(tmp_file(
                fs::temp_directory_path()))));
        }

        SECTION("directory") {
            REQUIRE_NOTHROW(path.reset(new fs::path(tmp_dir(
                fs::temp_directory_path()))));
        }

        REQUIRE(path != nullptr);
        REQUIRE_NOTHROW(wipe(*path));
        REQUIRE(is_wiped(*path));
    }
}
