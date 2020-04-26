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
 **/

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