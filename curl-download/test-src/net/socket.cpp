#include "net/socket.hpp"
#include <netinet/in.h>

#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

namespace net
{

socket::socket()
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        throw std::runtime_error("failed to create socket");
    }
}

socket::socket(int fd_)
: fd(fd_)
{

}


socket::~socket()
{
    close(fd);
}

socket::operator int () const
{
    return fd;
}

void socket::bind(int port)
{
    sockaddr_in address;
    memset(reinterpret_cast<void*>(&address), 0, sizeof(sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);

    int rc = ::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in));
    if (0 != rc)
    {
        throw std::runtime_error("failed to bind");
    }
}

void socket::listen(int backlog)
{
    int rc = ::listen(fd, backlog);
    if (0 != rc)
    {
        throw std::runtime_error("failed to listen");
    }
}

int socket::accept()
{
    int client_fd = ::accept(fd, nullptr, nullptr);
    return client_fd;
}

int socket::get_port() const
{
    sockaddr_in address;
    memset(reinterpret_cast<void*>(&address), 0, sizeof(sockaddr_in));
    socklen_t length = sizeof(sockaddr_in);

    int rc = getsockname(fd, reinterpret_cast<sockaddr*>(&address), &length);
    if (0 != rc)
    {
        throw std::runtime_error("failed to get port");
    }

    return ntohs(address.sin_port);
}

ssize_t socket::read(void * buffer, size_t length)
{
    return ::read(fd, buffer, length);
}

ssize_t socket::write(void const * buffer, size_t length)
{
    return ::write(fd, buffer, length);
}


}