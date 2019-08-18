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

#include <stdio.h>
#include <stdlib.h>

#include "image.h"

static void
print_usage(void)
{
    printf(
        "png_to_gray, (C) 2019 Falk Werner\n"
        "Turns png image to gray\n"
        "\n"
        "Usage:\n"
        "\tpng_to_gray <source.png> <target.png>\n"
        "\n"
        "Arguments:\n"
        "\tsource.png - source PNG file; will not be altered\n"
        "\ntarget.png - gray version of source.png\n"
        "\n"
        "Example:\n"
        "\tpng_turn_gray colored.png gray.png\n"
    );
}

static int
png_to_gray(
    char const * source,
    char const * target)
{
    struct image * image = image_load_from_file(source);
    if (NULL == image)
    {
        fprintf(stderr, "error: unable to read file \"%s\"\n", source);
        return EXIT_FAILURE;
    }

    int const width = image_get_width(image);
    int const height = image_get_height(image);

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            struct pixel pixel;
            image_get_pixel(image, x, y, &pixel);

            int color = (pixel.r + pixel.g + pixel.g) / 3;
            pixel.r = (unsigned char) color;
            pixel.g = (unsigned char) color;
            pixel.b = (unsigned char) color;

            image_set_pixel(image, x, y, &pixel);
        } 
    }

    int result = EXIT_SUCCESS;
    bool const is_saved = image_save_to_file(image, target);
    if (!is_saved)
    {
        fprintf(stderr, "error: unable to write file \"%s\"\n", target);
        result = EXIT_FAILURE;
    }

    image_release(image);
    return result;
}

int
main(int argc, char* argv[])
{
    int result = EXIT_SUCCESS;

    if (argc == 3)
    {
        char const * source = argv[1];
        char const * target = argv[2];

        result = png_to_gray(source, target);

    }
    else
    {
        print_usage();
    }
    


    return result;
}