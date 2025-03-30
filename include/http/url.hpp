#ifndef __URL_H__
#define __URL_H__

#include "http/utils.hpp"

namespace http {
namespace request {
    class Request;
}
class Url {
    friend class request::Request;    
public:
    Url() = default;
    Url(std::string_view path, std::string_view m_rawQuery) : m_path(path), m_rawQuery(m_rawQuery) {}
    const string& path() const { return m_path;}
    const string& raw_query() const {return m_rawQuery;}
    inline bool operator==(const Url& right) const {
        return m_path == right.m_path && m_rawQuery == right.m_rawQuery;
    }
private:
    string m_path;
    string m_rawQuery;
};
}

#endif // __URL_H__