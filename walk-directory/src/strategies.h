#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "walk_directory.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern walk_directory_fn *
walk_directory_get_strategy(
    char const * name);

extern int
walk_directory_readdir(
    char const * base,
    walk_directory_callback_fn * callback,
    void * user_data);

extern int
walk_directory_ftw(
    char const * base,
    walk_directory_callback_fn * callback,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif
