#include <iostream>
#include <http/server/server.hpp>
#include <http/request/request.hpp>

int main() {
    http::Server server(8080);
    http::request::Request::from_data("");
    server.start();
    return 0;
}