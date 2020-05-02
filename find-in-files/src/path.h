#ifndef PATH_H
#define PATH_H

#ifndef __cplusplus
#include <stddef.h>
#else
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct path
{
    char * name;
    size_t size;
    size_t capacity;
};

extern void
path_init(
    struct path * path,
    char const * name);

extern void
path_cleanup(
    struct path * path);

extern void
path_add(
    struct path * path,
    char const * name);

extern char const *
path_to_string(
    struct path * path);

extern int
path_save(
    struct path * path);

extern void
path_revert_to(
    struct path * path,
    int old_state);

#ifdef __cplusplus
}
#endif

#endif