#include "store.hpp"
#include "store_v0.0.2.hpp"

namespace ns_cipher {
    fs::path Store_v0_0_2::new_directory(fs::path const& path) {
        return path;
    }

    fs::path Store_v0_0_2::new_file(fs::path const& path) {
        return path;
    }

    void Store_v0_0_2::rm_directory(fs::path const& path) {
    }

    void Store_v0_0_2::rm_file(fs::path const& path) {
    }
}
