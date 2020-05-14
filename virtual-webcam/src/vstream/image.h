#ifndef IMAGE_H
#define IMAGE_H

#ifndef __cplusplus
#include <stddef.h>
#include <inttypes.h>
#else
#include <cstddef>
#include <cinttypes>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct image;

struct pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

extern struct image *
image_load_from_file(
    char const * filename);

extern void
image_release(
    struct image * image);

extern size_t
image_get_width(
    struct image const * image);

extern size_t
image_get_height(
    struct image const * image);

extern void
image_get_pixel(
    struct image const * image,
    size_t x,
    size_t y,
    struct pixel * pixel);

#ifdef __cplusplus
}
#endif

#endif
