#ifndef WIPE_HPP
#define WIPE_HPP
#pragma once

#include <boost/filesystem.hpp>

namespace ns_cipher {
    namespace fs = boost::filesystem;
    void wipe(fs::path const& path);
}

#endif
