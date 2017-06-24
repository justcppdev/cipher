#ifndef DECRYPTOR_HPP
#define DECRYPTOR_HPP
#pragma once

#include <boost/filesystem.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/operations.hpp>
#include <ios>
#include <cstdio>

namespace ns_cryptor {
    namespace fs = boost::filesystem;
    namespace io = boost::iostreams;

    class Decryptor final {
    public:
        Decryptor(fs::path const& path);
       ~Decryptor() = default;
        fs::path destination() const;

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

            auto result = static_cast<std::streamsize>(first - s);
            return result == 0 && c != io::WOULD_BLOCK ?
                -1 : result;
        }

    private:
        fs::path path_;
    };
}

#endif
