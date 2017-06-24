#include "decryptor.hpp"
#include <boost/filesystem.hpp>
#include <cassert>

namespace ns_cryptor {
    namespace fs = boost::filesystem;

    Decryptor::Decryptor(fs::path const& path): path_(path) {
        assert(fs::exists(path));
    }

    fs::path Decryptor::destination() const {

        if (path_.extension() == ".x") {
            return fs::path(path_).replace_extension();

        } else {
            return fs::path(path_);
        }
    }
}
