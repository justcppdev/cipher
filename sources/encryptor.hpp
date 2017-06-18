#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP
#pragma once

#include <boost/iostreams/categories.hpp>
#include <ios>

namespace ns_cryptor {
    namespace io = boost::iostreams;
    class Encryptor final {
    public:
        using char_type = char;
        using category = io::bidirectional_device_tag;

        std::streamsize read(char const *s, std::streamsize n);
        void write(char const *s, std::streamsize n);
    };
}

#endif
