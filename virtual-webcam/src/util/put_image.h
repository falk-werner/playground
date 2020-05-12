#ifndef PUT_IMAGE_H
#define PUT_IMAGE_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern bool
put_image(
    char const * video_device,
    char const * image_filename,
    char * * error_message);

#ifdef __cplusplus
}
#endif

#endif
