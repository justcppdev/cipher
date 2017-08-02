#ifndef STORE_v0_0_2_HPP
#define STORE_v0_0_2_HPP
#pragma once

#include "store.hpp"

namespace ns_cipher {
    class Store_v0_0_2 final: public Store {
    public:
        fs::path new_directory(fs::path const& path) override;
        fs::path new_file(fs::path const& path) override;
        void rm_directory(fs::path const& path) override;
        void rm_file(fs::path const& path) override;
    };
}

#endif
