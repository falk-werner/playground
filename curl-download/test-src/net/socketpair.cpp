#include "net/socketpair.hpp"

#include <sys/socket.h>
#include <stdexcept>

#define SOCKETPAIR_READ_FD 0
#define SOCKETPAIR_WRITE_FD 1

namespace net
{

socketpair::socketpair()
{
    int rc = ::socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
    if (0 != rc)
    {
        throw std::runtime_error("failed to create socketpair");
    }
}

socketpair::~socketpair()
{
    close(fds[SOCKETPAIR_READ_FD]);
    close(fds[SOCKETPAIR_WRITE_FD]);
}

int socketpair::get_readfd() const
{
    return fds[SOCKETPAIR_READ_FD];
}

ssize_t socketpair::write(void const * buffer, size_t length)
{
    return ::write(fds[SOCKETPAIR_WRITE_FD], buffer, length);
}

ssize_t socketpair::read(void * buffer, size_t length)
{
    return ::read(fds[SOCKETPAIR_READ_FD], buffer, length);
}

}