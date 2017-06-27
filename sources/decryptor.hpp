#ifndef DECRYPTOR_HPP
#define DECRYPTOR_HPP
#pragma once

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/operations.hpp>
#include <ios>
#include <string>

namespace ns_cryptor {
    namespace io = boost::iostreams;

    class Decryptor final {
    public:
        Decryptor(std::string const& password);
       ~Decryptor() = default;

        using char_type = char;
        using category = io::multichar_input_filter_tag;

        template<typename Source>
        std::streamsize read(Source& src, char* s, std::streamsize n) {
            char* first = s;
            char* last  = s + n;
            int   c;

            while(first != last &&
                  (c = io::get(src)) != EOF && c != io::WOULD_BLOCK) {
                *first++ = c ^ 0x01010101;
            }

            auto readed = static_cast<std::streamsize>(first - s);
            return readed == 0 && c != io::WOULD_BLOCK ?
                -1 : readed;
        }

    private:
        std::string aes_key;
        std::string crc_aes;
    };
}

#endif
