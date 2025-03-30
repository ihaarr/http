#include <algorithm>
#include <string_view>
#include "http/request/request.hpp"
#include "http/methods.hpp"

namespace http::request {
    expected<Request, ParseError> Request::from_data(std::string_view data) {
        constexpr std::size_t kCRLFLen = 2;
        constexpr std::size_t kSpaceLen = 1;
        Request req;
        auto curPos = data.begin(); 

        //Parsing request line
        auto lineEnd = std::find(curPos, data.end(), '\r');
        if (lineEnd == data.end()) {
            LOG_DEBUG("Failed to find clrf");
            return tl::unexpected{ParseError::BadRequestLine};
        }
        LOG_DEBUG("Found end of request line");
        auto spIter = std::find(curPos, data.end(), ' ');
        if (spIter == data.end()) {
            LOG_DEBUG("Failed to parse method");
            return tl::unexpected{ParseError::BadMethod};
        }
        LOG_DEBUG("Found space after method");
        auto method = std::string_view{data.data() + std::distance(data.begin(), curPos), std::distance(data.begin(), spIter)};
        req.m_method = string_to_method(method);
        std::advance(curPos, method.size() + 1);
        auto uriEndIter = std::find(curPos, data.end(), ' ');
        if (uriEndIter == data.end()) {
            LOG_DEBUG("Failed to parse uri");
            return tl::unexpected{ParseError::BadUri};
        }
        LOG_DEBUG("Found space after Uri");
        auto queryBegin = std::find(curPos, uriEndIter, '?');
        if (queryBegin != data.end()) {
            req.m_url.m_path = string{curPos, queryBegin};
            req.m_url.m_rawQuery = string{queryBegin, uriEndIter};
            std::advance(curPos, req.m_url.m_path.size() + req.m_url.m_rawQuery.size() + kSpaceLen);
        } else {
            req.m_url.m_path = string{curPos, uriEndIter};
            std::advance(curPos, req.m_url.m_path.size()); 
        }
        LOG_DEBUG("Parsed query");
        auto len = std::distance(curPos, data.end());
        if (len < 5) {
            LOG_DEBUG("Failed to parse protocol");
            return tl::unexpected{ParseError::BadPrefixProtocol};
        }
        req.m_proto = string_to_procol(std::string_view{curPos, std::distance(curPos, lineEnd)}); 
        std::advance(curPos, std::distance(curPos, lineEnd) + kCRLFLen);
        LOG_DEBUG("Parsed protocol");
        for(;;) {
            lineEnd = std::find(curPos, data.end(), '\r');
            if (lineEnd == data.end()) {
                LOG_DEBUG("Failed to parse header line");
                return tl::unexpected(ParseError::BadCRLFHeader);
            } else if (curPos == lineEnd) {
                std::advance(curPos, 2); // 2 == \r\n
                break;
            }
            auto colonIter = std::find(curPos, lineEnd, ':');
            if (colonIter == data.end()) {
                LOG_DEBUG("Failed to parse header");
                return tl::unexpected(ParseError::BadHeader);
            }

            auto header = string(curPos, colonIter);
            auto headerLen = header.size();
            LOG_DEBUG("Got header name");
            auto value = string(colonIter + 2, lineEnd);
            auto valLen = value.size();
            LOG_DEBUG("Got header value");
            req.m_headers.insert(std::make_pair(std::move(header), std::move(value))); // 2 = \:\SPACE
            std::advance(curPos, std::distance(curPos, lineEnd) + kCRLFLen); 
        }
        LOG_DEBUG("Parsed headers");

        if (req.m_headers.count("Content-Length")) {
            size_t len = std::stoi(req.m_headers.at("Content-Length"));
            if (len < 0) {
                LOG_DEBUG("Got invalid content length");
                return tl::unexpected(ParseError::InvalidContentLength);
            }
            //TODO: while function waits all response, data can be transfered not all 
            if (std::distance(curPos, data.end()) < len) {
                return req;
            }

            req.m_body = std::vector<byte>{curPos, curPos + len + 1};
        }
        LOG_DEBUG("Parsed body");
        return req;
    }
    const std::vector<byte>& Request::body() const {
        return m_body;
    }
    Method Request::method() const {
        return m_method;
    }
    const Url& Request::url() const {
        return m_url;
    }
    optional<const string&> Request::header(const string& key) const {
        if (m_headers.count(key)) {
            return m_headers.at(key);
        }
        return boost::none; 
    }
}