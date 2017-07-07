#ifndef TEST_HELPER_HPP
#define TEST_HELPER_HPP
#pragma once

#include <boost/filesystem.hpp>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    fs::path tmp_file(fs::path const& dir);
    fs::path tmp_dir(fs::path const& dir);
    bool is_wiped(fs::path const& path);
}

#endif
