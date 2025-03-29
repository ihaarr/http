#ifndef __SERVER_H__
#define __SERVER_H__

#include <boost/asio.hpp>
#include "http/utils.hpp"

namespace http {
class Server final {
public:
    Server(u16 port);

    void start();
private:
    void handler(boost::asio::ip::tcp::socket& socket);
private:
    u16 m_port;
};
}

#endif // __SERVER_H__