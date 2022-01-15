#ifndef FETCH_H
#define FETCH_H

#ifdef __cplusplus
extern "C"
{
#endif

struct fetch_handle;

extern struct fetch_handle *
fetch(
    char const * url,
    char const * filename);

extern void
fetch_release(
    struct fetch_handle * handle);

extern int
fetch_wait(
    struct fetch_handle * handle);

extern void
fetch_cancel(
    struct fetch_handle * handle);

#ifdef __cplusplus
}
#endif

#endif
