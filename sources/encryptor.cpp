#include "encryptor.hpp"
#include <boost/filesystem.hpp>
#include <cassert>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    Encryptor::Encryptor(fs::path const& path): path_(path) {
        assert(fs::exists(path));
    }

    fs::path Encryptor::destination() const {
        return fs::path(path_) += ".x";
    }
}
