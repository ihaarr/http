#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>
#include <string>
#include <boost/core/span.hpp>

namespace http {
using byte = std::uint8_t;
using u16 = std::uint16_t;

using string = std::string;
template<typename T>
using span = boost::span<T>;
};

#endif // __UTILS_H__