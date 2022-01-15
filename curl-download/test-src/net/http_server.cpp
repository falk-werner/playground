#include "net/http_server.hpp"

#include <sys/socket.h>
#include <poll.h>

#include <iostream>
#include <exception>

namespace
{

void http_server_run(net::http_server * server)
{
    server->run();
}

}

namespace net
{

http_server::http_server(std::function<std::string (std::string const&)>handler_)
: handler(handler_)
{
    server.bind();
    server.listen();

    thread = std::thread(http_server_run, this);
}

http_server::~http_server()
{
    char c = 'q';
    pair.write(reinterpret_cast<void*>(&c), 1);
    thread.join();
}

int http_server::get_port() const
{
    return server.get_port();
}

void http_server::run()
{
    pollfd fds[2];
    fds[0].fd = pair.get_readfd();
    fds[0].events = POLLIN;
    fds[1].fd = server;
    fds[1].events = POLLIN;

    bool shutdown_requested = false;
    while (!shutdown_requested)
    {
        int rc =poll(fds, 2, 1000);
        if (0 < rc)
        {
            if (0 != fds[0].revents)
            {
                char c;
                pair.read(reinterpret_cast<void*>(&c), 1);
                shutdown_requested = true;
            }

            if (0 != (fds[1].revents & POLLIN))
            {
                std::cout << "server events" << std::endl;
                int client_fd = server.accept();
                if (client_fd >= 0)
                {
                    socket client(client_fd);
                    process_client(client);
                }
                else
                {
                    perror("accept");
                }
            }
        }
    }
}

void http_server::process_client(net::socket & client)
{
    std::cout << "process client" << std::endl;
    char header[4096];
    ssize_t length = client.read(reinterpret_cast<void*>(header), 4095);
    if (length > 0)
    {
        header[length] = '\0';
        std::cout << header << std::endl;
        auto response = handler(header);
        client.write(reinterpret_cast<void const*>(response.c_str()), response.size());
    }
}


}