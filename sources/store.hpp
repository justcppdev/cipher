#ifndef STORE_HPP
#define STORE_HPP
#pragma once

#include <boost/filesystem.hpp>
#include <memory>
#include <string>

namespace ns_cipher {
    namespace fs = boost::filesystem;

    class Store {
    protected:
        Store() = default;
    public:
        Store(Store const& rhs) = delete;
        Store& operator=(Store const& rhs) = delete;
        Store(Store&& rhs) = default;
        Store& operator=(Store&& rhs) = default;
        virtual ~Store() = default;

        virtual fs::path new_directory(fs::path const& path) = 0;
        virtual fs::path new_file(fs::path const& path) = 0;
        virtual void rm_directory(fs::path const& path) = 0;
        virtual void rm_file(fs::path const& path) = 0;
    };

    std::unique_ptr<Store>
    make_store(std::string const& version);
}

#endif

