#ifndef NET_HTTP_SERVER_H
#define NET_HTTP_SERVER_H

#include "net/socket.hpp"
#include "net/socketpair.hpp"

#include <string>
#include <thread>
#include <functional>

namespace net
{


class http_server
{
public:
    http_server(std::function<std::string (std::string const&)> handler);
    ~http_server();
    int get_port() const;

    void run();
private:
    void process_client(net::socket & socket);

    std::function<std::string (std::string const&)> handler;
    net::socket server;
    net::socketpair pair;
    std::thread thread;
};

}

#endif

