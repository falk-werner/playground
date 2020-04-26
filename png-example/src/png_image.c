/**
 * MIT License
 * 
 * Copyright (c) 2019 Falk Werner
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Note:
 * The implementation of the core functions, image_load_from_file and image_save_to_file, 
 * is strongly inspired by https://gist.github.com/niw/5963798.
 **/

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#define PIXEL_SIZE  4
#define RED_INDEX   0
#define GREEN_INDEX 1
#define BLUE_INDEX  2
#define ALPHA_INDEX 3

struct image
{
    int width;
    int height;
    png_bytep * rows;
};

extern struct image *
image_create(
    int width,
    int height
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

bool
image_save_to_file(
    struct image * image,
    char const const * filename)
{
    FILE * file = fopen(filename, "wb");
    if (NULL == file)
    {
        return false;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fclose(file);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_write_struct(&png, NULL);
        fclose(file);
        return false;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        return false;
    }

    png_init_io(png, file);

    png_set_IHDR(
        png,
        info,
        image->width,
        image->height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    png_write_image(png, image->rows);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(file);

    return true;
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
    if ((0 <= x) && (x < image->width) && (0 <= y) && (y < image->height))
    {
        png_bytep row = image->rows[y];        
        png_bytep source = &(row[x * PIXEL_SIZE]);
        pixel->r = source[RED_INDEX];
        pixel->g = source[GREEN_INDEX];
        pixel->b = source[BLUE_INDEX];
        pixel->a = source[ALPHA_INDEX];
    }
}

void
image_set_pixel(
    struct image const * image,
    int x,
    int y,
    struct pixel const * pixel)
{
    if ((0 <= x) && (x < image->width) && (0 <= y) && (y < image->height))
    {
        png_bytep row = image->rows[y];        
        png_bytep target = &(row[x * PIXEL_SIZE]);
        target[RED_INDEX]   = pixel->r;
        target[GREEN_INDEX] = pixel->g;
        target[BLUE_INDEX]  = pixel->b;
        target[ALPHA_INDEX] = pixel->a;
    }
}
