#include "pidfd_open.h"

#include <sys/syscall.h>
#include <unistd.h>

#ifndef SYS_pidfd_open
#define SYS_pidfd_open 434   /* System call # on most architectures */
#endif

int pidfd_open(pid_t pid, int flags)
{
    return syscall(SYS_pidfd_open, pid, flags);
}
