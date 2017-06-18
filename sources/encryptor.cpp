#include "encryptor.hpp"

namespace ns_cryptor {
    std::streamsize Encryptor::read(char const *s, std::streamsize n) {
        return n;
    }

    void Encryptor::write(char const *s, std::streamsize n) {
    }
}
