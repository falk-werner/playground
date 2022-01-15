#ifndef NET_SOCKETPAIR_HPP
#define NET_SOCKETPAIR_HPP

#include <sys/types.h>
#include <unistd.h>


namespace net
{

class socketpair
{
public:
    socketpair();
    ~socketpair();
    int get_readfd() const;
    ssize_t write(void const * buffer, size_t length);
    ssize_t read(void * buffer, size_t length);
private:
    int fds[2];
};

}

#endif
