#include "image.h"
#include <stdlib.h>

struct image
{
    int width;
    int height;
};

extern struct image *
image_create(
    int width,
    int height
)
{
    // ToDo: implement me
    struct image * image = malloc(sizeof(struct image));
    image->width = width;
    image->height = height;

    return image;
}

void 
image_release (
    struct image * image)
{
    // ToDo: implement me
    free(image);
}

struct image *
image_load_from_file(
    char const * filename)
{
    // ToDo: implement me
    return NULL;
}

bool
image_save_to_file(
    struct image * image,
    char const const * filename)
{
    // ToDo: implement me
    return false;
}

int
image_get_width(
    struct image const * image)
{
    return image->width;
}

int
image_get_height(
    struct image const * image)
{
    return image->height;
}

void
image_get_pixel(
    struct image const * image,
    int x,
    int y,
    struct pixel * pixel)
{
    // ToDo: implement me
}

void
image_set_pixel(
    struct image const * image,
    int x,
    int y,
    struct pixel const * pixel)
{
    // ToDo: implement me
}