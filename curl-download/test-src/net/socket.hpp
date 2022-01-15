#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <sys/types.h>
#include <unistd.h>

namespace net
{

class socket
{
public:
    socket();
    socket(int fd);
    ~socket();
    operator int () const;
    void bind(int port = 0);
    void listen(int backlog = 5);
    int accept();
    int get_port() const;
    ssize_t read(void * buffer, size_t length);
    ssize_t write(void const * buffer, size_t length);
private:
    int fd;
};

}

#endif
