#define _GNU_SOURCE

#include "put_image.h"
#include "image.h"
#include "yuv.h"

#include <linux/videodev2.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

static void
report_error(
    char * * error_message,
    char const * format,
    ...)
{
    if (NULL != error_message)
    {
        va_list args;
        va_start(args, format);
        vasprintf(error_message, format, args);
        va_end(args);
    }
}

extern bool
put_image(
    char const * video_device,
    char const * image_filename,
    char * * error_message)
{
    bool result = true;

    size_t width = 0;
    size_t height = 0;
    struct image * image = image_load_from_file(image_filename);
    if (NULL != image)
    {
        size_t width = image_get_width(image);
        size_t height = image_get_height(image);
    }
    else
    {
        report_error(error_message, "failed to load image %s", image_filename);
        result = false;
    }

    int fd = 0;
    if (result)
    {
        fd = open(video_device, O_RDWR);
        if (0 >= fd)
        {
            report_error(error_message, "failed to open video device: %s", video_device);
            result = false;
        }
    }

    size_t frame_size = 0;
    struct v4l2_format video_format;
    if (result)
    {
        frame_size = 3 * width * height / 2;

        memset(&video_format, 0, sizeof(struct v4l2_format));
        video_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        video_format.fmt.pix.width = width;
        video_format.fmt.pix.height = height;
        video_format.fmt.pix.sizeimage = frame_size;
        video_format.fmt.pix.bytesperline = width;
        video_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
        video_format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
        video_format.fmt.pix.field = V4L2_FIELD_NONE;
        video_format.fmt.pix.priv = V4L2_PIX_FMT_PRIV_MAGIC;

        int rc = ioctl(fd, VIDIOC_S_FMT, &video_format);
        if (-1 == rc)
        {
            report_error(error_message, "failed to write video format");
            result = false;
        }
    }


    uint8_t * frame_buffer = NULL;
    if (result)
    {
        frame_size = video_format.fmt.pix.sizeimage;
        width = video_format.fmt.pix.width;
        height = video_format.fmt.pix.height;
        size_t offset_u = width * height;
        size_t offset_v = offset_u + (offset_u / 4);

        frame_buffer = malloc(frame_size);
        memset(frame_buffer, 0, frame_size);
        for(size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                size_t i = (y * width) + x;

                struct pixel pixel = { 0, 0, 0};
                image_get_pixel(image, x, y, &pixel);
                struct yuv yuv;
                rgb_to_yuv(&pixel, &yuv);

                frame_buffer[i] = yuv.y;
                if ((0 == (x % 2)) && (0 == (y % 2)))
                {
                    size_t uv_i = ((y / 2) * (width / 2)) + (x/2);
                    frame_buffer[uv_i + offset_u] = yuv.u;
                    frame_buffer[uv_i + offset_v] = yuv.v;
                }
            }
        }

        ssize_t bytes_written = write(fd, frame_buffer, frame_size);
        if (bytes_written != frame_size)
        {
            report_error(error_message, "failed to write frame");
            result = false;
        }
    }

    if (NULL != frame_buffer) { free(frame_buffer); }
    if (0 < fd) { close(fd); }
    if (NULL != image) { image_release(image); }

    return result;
}
