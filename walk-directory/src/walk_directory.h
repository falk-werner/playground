#ifndef WALK_DIRECTORY_STRATEGY_H
#define WALK_DIRECTORY_STRATEGY_H

#ifdef __cplusplus
extern "C"
{
#endif

#define FILETYPE_DIR 1
#define FILETYPE_REG 2

typedef int 
walk_directory_callback_fn(
    void * user_data,
    int type,
    char const * name);

typedef int
walk_directory_fn(
    char const * base,
    walk_directory_callback_fn * callback,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif
