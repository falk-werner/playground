#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct video_stream;

extern struct video_stream *
video_stream_create(
    char const * video_device);

extern bool
video_stream_put_image(
    struct video_stream * stream,
    char const * image_filename,
    char * * error_message);

extern void
video_stream_close(
    struct video_stream * stream);

#ifdef __cplusplus
}
#endif

#endif
