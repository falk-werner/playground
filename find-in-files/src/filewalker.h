#ifndef FILEWALKER_H
#define FILEWALKER_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void filewalker_callback_fn(
    void * user_data,
    int parent_fd,
    char const * filename,
    char const * full_filename);

extern void
filewalker_walk(
    char const * base_directory,
    filewalker_callback_fn * callback,
    void * user_data);

#ifdef __cplusplus
}
#endif

#endif
