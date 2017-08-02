#include "store_v0.0.2.hpp"

namespace ns_cipher {
    std::unique_ptr<Store>
    make_store(std::string const& version) {
        if (version == "0.0.2") {
            return std::make_unique<Store_v0_0_2>();
        } else {
            throw std::string("unknown version: ") += version;
        }
    }
}
