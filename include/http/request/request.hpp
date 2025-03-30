#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <map>
#include <vector>
#include "http/methods.hpp"
#include "http/url.hpp"

namespace http::request {
enum class ParseError : byte {
    BadMethod,
    BadRequestLine,
    BadUri,
    BadPrefixProtocol,
    BadHeader,
    BadCRLFHeader,
    InvalidContentLength
};
enum class Protocol {
    HTTP1_1,
    Unknown
};

inline Protocol string_to_procol(std::string_view s) {
    if (s == "HTTP/1.1") return Protocol::HTTP1_1;
    return Protocol::Unknown;
}
class Request {
public:
    Request() = default;
    static expected<Request, ParseError> from_data(std::string_view data);

    Method method() const;
    const Url& url() const;
    optional<const string&> header(const string& key) const;
    const std::vector<byte>& body() const;
private:
    Protocol m_proto;
    Method m_method;
    Url m_url;
    std::map<string, string> m_headers;
    std::vector<byte> m_body;
};
}

#endif // __REQUEST_H__