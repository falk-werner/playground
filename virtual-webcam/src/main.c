#include "image.h"

#include <linux/videodev2.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

struct context
{
    char const * device;
    char const * filename; 
};

#define REQ_WITDH  640
#define REQ_HEIGHT 480

struct rgb { uint8_t r; uint8_t g; uint8_t b; };
struct yuv { uint8_t y; uint8_t u; uint8_t v; };

static uint8_t clamp(double value)
{
    if (0 > value) { return 0; }
    if (255 < value) { return 255; }
    return (uint8_t) value;
}

static void rgb_to_yuv(struct pixel const * rgb, struct yuv * yuv)
{
    double r = rgb->r;
    double g = rgb->g;
    double b = rgb->b;

    double y = ( 0.299000 * r) + ( 0.587000 * g) + ( 0.114000 * b);
    double u = (-0.168736 * r) + (-0.331264 * g) + ( 0.500000 * b) + 128;
    double v = ( 0.500000 * r) + (-0.418688 * g) + (-0.081312 * b) + 128;

    yuv->y = clamp(y);
    yuv->u = clamp(u);
    yuv->v = clamp(v);
}



int main(int argc, char * argv[])
{   
    if (argc < 2)
    {
        printf("usage: test <image.png>\n");
        exit(EXIT_FAILURE);
    }

    char const * video_device = "/dev/video0";
    int fd = open(video_device, O_RDWR);
    if (0 >= fd)
    {
        fprintf(stderr, "error: failed to open video device: %s\n", video_device);
        exit(EXIT_FAILURE);
    }

    struct v4l2_capability video_capavilities;
    memset(&video_capavilities, 0, sizeof(struct v4l2_capability));
    int rc = ioctl(fd, VIDIOC_QUERYCAP, &video_capavilities);
    if (-1 == rc)
    {
        fprintf(stderr, "error: failed to query video capabilities\n");
    }

    struct v4l2_format video_format;
    memset(&video_format, 0, sizeof(struct v4l2_format));
    video_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    rc = ioctl(fd, VIDIOC_G_FMT, &video_format);
    if (-1 == rc)
    {
        fprintf(stderr, "error: failed to read video format\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t requested_frame_size = 3 * REQ_WITDH * REQ_HEIGHT / 2;

    memset(&video_format, 0, sizeof(struct v4l2_format));
    video_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    video_format.fmt.pix.width = REQ_WITDH;
    video_format.fmt.pix.height = REQ_HEIGHT;
    video_format.fmt.pix.sizeimage = requested_frame_size;
    video_format.fmt.pix.bytesperline = REQ_WITDH;
    video_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
    video_format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
    video_format.fmt.pix.field = V4L2_FIELD_NONE;

    rc = ioctl(fd, VIDIOC_S_FMT, &video_format);
    if (-1 == rc)
    {
        fprintf(stderr, "error: failed to write video format\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t frame_size = video_format.fmt.pix.sizeimage;
    size_t width = video_format.fmt.pix.width;
    size_t height = video_format.fmt.pix.height;
    size_t offset_u = width * height;
    size_t offset_v = offset_u + (offset_u / 4);

    printf("debug: video size: width: %zu, height: %zu\n", width, height);

    char const * image_file = argv[1];
    struct image * image = image_load_from_file(image_file);
    if (NULL == image)
    {
        fprintf(stderr, "error: failed to load image %s\n", image_file);
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("debug: image size: width: %zu, height: %zu\n", image_get_width(image), image_get_height(image));

    uint8_t * frame_buffer = malloc(frame_size);
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
    image_release(image);

    ssize_t bytes_written = write(fd, frame_buffer, frame_size);
    if (bytes_written != frame_size)
    {
        fprintf(stderr, "error: failed to write frame\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return EXIT_SUCCESS;
}