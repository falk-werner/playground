#ifndef MD5_H
#define MD5_H

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct md5;

extern struct md5 *
md5_create(void);

extern void
md5_update(
    struct md5 * md5,
    char const * data,
    size_t length);

extern char *
md5_finalize(
    struct md5 * md5);

#ifdef __cplusplus
}
#endif

#endif
