#define _GNU_SOURCE

#include "video_stream.h"
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

struct video_stream
{
    bool is_initialized;
    size_t width;
    size_t height;
    size_t frame_size;
    uint8_t * frame_buffer;
    int fd;
};

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

struct video_stream *
video_stream_create(
    char const * video_device)
{
    struct video_stream * stream = malloc(sizeof(struct video_stream));
    stream->is_initialized = false;
    stream->fd = open(video_device, O_RDWR);
    stream->frame_buffer = NULL;
    if (0 >= stream->fd)
    {
        free(stream);
        stream = NULL;
    }

    return stream;
}

void
video_stream_close(
    struct video_stream * stream)
{
    close(stream->fd);
    free(stream->frame_buffer);
    free(stream);
}


bool
video_stream_put_image(
    struct video_stream * stream,
    char const * image_filename,
    char * * error_message)
{
    bool result = true;

    struct image * image = image_load_from_file(image_filename);
    if (NULL == image)
    {
        report_error(error_message, "failed to load image %s", image_filename);
        result = false;
    }

    if ((result) && (!stream->is_initialized))
    {
        struct v4l2_format video_format;
        size_t width = image_get_width(image);
        size_t height = image_get_height(image);
        size_t frame_size = 3 * width * height / 2;

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

        int rc = ioctl(stream->fd, VIDIOC_S_FMT, &video_format);
        if (-1 != rc)
        {
            stream->frame_size = video_format.fmt.pix.sizeimage;
            stream->width = video_format.fmt.pix.width;
            stream->height = video_format.fmt.pix.height;
            stream->frame_buffer = malloc(stream->frame_size);

            stream->is_initialized = true;
        }
        else
        {
            report_error(error_message, "failed to write video format");
            result = false;
        }
    }


    if (result)
    {
        size_t offset_u = stream->width * stream->height;
        size_t offset_v = offset_u + (offset_u / 4);

        for(size_t y = 0; y < stream->height; y++)
        {
            for (size_t x = 0; x < stream->width; x++)
            {
                size_t i = (y * stream->width) + x;

                struct pixel pixel = { 0, 0, 0};
                image_get_pixel(image, x, y, &pixel);
                struct yuv yuv;
                rgb_to_yuv(&pixel, &yuv);

                stream->frame_buffer[i] = yuv.y;
                if ((0 == (x % 2)) && (0 == (y % 2)))
                {
                    size_t uv_i = ((y / 2) * (stream->width / 2)) + (x/2);
                    stream->frame_buffer[uv_i + offset_u] = yuv.u;
                    stream->frame_buffer[uv_i + offset_v] = yuv.v;
                }
            }
        }

        ssize_t bytes_written = write(stream->fd, stream->frame_buffer, stream->frame_size);
        if (bytes_written != stream->frame_size)
        {
            report_error(error_message, "failed to write frame");
            result = false;
        }
    }

    if (NULL != image) { image_release(image); }

    return result;
}
