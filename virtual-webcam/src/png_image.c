#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#define PIXEL_SIZE  4
#define RED_INDEX   0
#define GREEN_INDEX 1
#define BLUE_INDEX  2

struct image
{
    size_t width;
    size_t height;
    png_bytep * rows;
};

static struct image *
image_create(
    size_t width,
    size_t height
)
{
    struct image * image = malloc(sizeof(struct image));
    image->width = width;
    image->height = height;
    image->rows = malloc(sizeof(png_bytep) * height);
    
    int const row_size = width * sizeof(png_byte) * PIXEL_SIZE;
    for(int y = 0; y < height; y++)
    {
        image->rows[y] = malloc(row_size);
    }

    return image;
}

void 
image_release (
    struct image * image)
{
    if (NULL != image)
    {
        for(int y = 0; y < image->height; y++)
        {
            free(image->rows[y]);
        }
        free(image->rows);
        free(image);
    }
}

struct image *
image_load_from_file(
    char const * filename)
{
    struct image * image = NULL;

    FILE * file = fopen(filename, "rb");
    if (NULL == file)
    {
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fclose(file);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(file);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        image_release(image);        
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        return NULL;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    int const width = png_get_image_width(png, info);
    int const height = png_get_image_height(png, info);
    image = image_create(width, height);

    png_byte const bit_depth = png_get_bit_depth(png, info);
    if (16 == bit_depth)
    {
        png_set_strip_16(png);
    }

    png_byte const color_type = png_get_color_type(png, info);
    if (PNG_COLOR_TYPE_PALETTE == color_type)
    {
        png_set_palette_to_rgb(png);
    }

    if ((PNG_COLOR_TYPE_GRAY == color_type) && (bit_depth < 8))
    {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }

    if ((PNG_COLOR_TYPE_RGB == color_type) ||
        (PNG_COLOR_TYPE_GRAY == color_type) ||
        (PNG_COLOR_TYPE_PALETTE == color_type))
    {
        png_set_filler(png, 0xff, PNG_FILLER_AFTER);
    }

    if ((PNG_COLOR_TYPE_GRAY == color_type) ||
        (PNG_COLOR_TYPE_GRAY_ALPHA == color_type))
    {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    png_read_image(png, image->rows);

    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);

    return image;
}

size_t
image_get_width(
    struct image const * image)
{
    return image->width;
}

size_t
image_get_height(
    struct image const * image)
{
    return image->height;
}

void
image_get_pixel(
    struct image const * image,
    size_t x,
    size_t y,
    struct pixel * pixel)
{
    if ((x < image->width) && (y < image->height))
    {
        png_bytep row = image->rows[y];        
        png_bytep source = &(row[x * PIXEL_SIZE]);
        pixel->r = source[RED_INDEX];
        pixel->g = source[GREEN_INDEX];
        pixel->b = source[BLUE_INDEX];
    }
}
