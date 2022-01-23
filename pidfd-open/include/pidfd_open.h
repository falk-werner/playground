#ifndef PIDFD_OPEN_H
#define PIDFD_OPEN_H

#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern int pidfd_open(pid_t pid, int flags);

#ifdef __cplusplus
}
#endif

#endif
