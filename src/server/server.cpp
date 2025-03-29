#include <iostream>
#include <array>
#include "http/server/server.hpp"

namespace http {
using namespace boost::asio;
Server::Server(u16 port) : m_port(port) {
}
void Server::handler(ip::tcp::socket& socket) {
    std::array<byte, 1024> buf;
    boost::system::error_code ec;
    do {
        auto readed = boost::asio::read(socket, buffer(buf, buf.size()), boost::asio::transfer_at_least(1), ec);
        boost::asio::write(socket, buffer(buf, readed), ec);
    } while(!ec);
}
void Server::start() {
    io_context io_context;
    ip::tcp::acceptor acceptor{ io_context,
                                ip::tcp::endpoint(ip::tcp::v4(), m_port) };
    bool running = true;
    std::cout << "Running server on " << acceptor.local_endpoint().address() << ':' << m_port << '\n';
    while(running) {
        auto socket = ip::tcp::socket(io_context);
        acceptor.accept(socket);
        handler(socket);
    }
}
}