#ifndef IMAGE_H
#define IMAGE_H

#ifndef __cplusplus
#include <stdbool.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif


struct image;

struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

extern struct image *
image_create(
    int width,
    int height
);

extern void 
image_release (
    struct image * image);

extern struct image *
image_load_from_file(
    char const * filename);

extern bool
image_save_to_file(
    struct image * image,
    char const const * filename);

extern int
image_get_width(
    struct image const * image);

extern int
image_get_height(
    struct image const * image);

extern void
image_get_pixel(
    struct image const * image,
    int x,
    int y,
    struct pixel * pixel);

extern void
image_set_pixel(
    struct image const * image,
    int x,
    int y,
    struct pixel const * pixel);

#ifdef __cplusplus
}
#endif

#endif