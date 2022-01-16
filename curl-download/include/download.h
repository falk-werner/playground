#ifndef FETCH_H
#define FETCH_H

#ifdef __cplusplus
extern "C"
{
#endif

struct download;

extern struct download *
download_create(
    char const * url,
    char const * filename);

extern void
download_release(
    struct download * handle);

extern int
download_perform(
    struct download * handle);

extern void
download_cancel(
    struct download * handle);

#ifdef __cplusplus
}
#endif

#endif
