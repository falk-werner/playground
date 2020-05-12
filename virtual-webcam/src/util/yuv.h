#ifndef YUV_H
#define YUV_H

#ifndef __cplusplus
#include <inttypes.h>
#else
#include <cinttypes>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct pixel;

struct yuv
{
    uint8_t y;
    uint8_t u;
    uint8_t v; 
};

extern void
rgb_to_yuv(
    struct pixel const * rgb,
    struct yuv * yuv);

#ifdef __cplusplus
}
#endif

#endif
