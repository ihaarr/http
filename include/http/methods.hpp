#ifndef __METHODS_H__
#define __METHODS_H__

#include <string_view>
#include <iostream>
#include "http/utils.hpp"

namespace http {
enum class Method : byte {
    GET,
    POST,
    PUT,
    DELETE,
    UNSUPPORTED
};

inline Method string_to_method(std::string_view s) {
    auto comparator = [](byte a, byte b) {
        return std::tolower(static_cast<unsigned char>(a)) ==
            std::tolower(static_cast<unsigned char>(b));
    };
    auto comp = [&comparator](std::string_view a, std::string_view b) {
        return std::equal(a.begin(), a.end(), b.begin(), comparator);
    };
    auto get = std::string_view("get");
    auto post = std::string_view("post");
    auto put = std::string_view("put");
    auto delet = std::string_view("delete");

    if (comp(s, get)) return Method::GET;
    if (comp(s, post)) return Method::POST;
    if (comp(s, put)) return Method::PUT;
    if (comp(s, delet)) return Method::DELETE;
    return Method::UNSUPPORTED;
}
}

#endif // __METHODS_H__