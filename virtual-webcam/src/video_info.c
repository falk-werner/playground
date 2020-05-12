#include <linux/videodev2.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

static const char *
get_pixel_format(uint32_t value, char * buffer)
{
    sprintf(buffer, "%c%c%c%c",
        (char) ((value      ) & 0xff),
        (char) ((value >>  8) & 0xff),
        (char) ((value >> 16) & 0xff),
        (char) ((value >> 24) & 0xff)
    );

    return buffer;
}

int main(int argc, char * argv[])
{
    if (1 < argc) 
    {
        char const * video_device = argv[1];

//        int fd = open(video_device, O_RDWR);
        int fd = open(video_device, O_RDONLY);
        if (0 >= fd)
        {
            fprintf(stderr, "error: failed to open video format\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        struct v4l2_format video_format;
        memset(&video_format, 0, sizeof(struct v4l2_format));
        video_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        int rc = ioctl(fd, VIDIOC_G_FMT, &video_format);
        if (-1 == rc)
        {
            fprintf(stderr, "error: failed to get video format\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        char buffer[5];
        printf("width : %u\n", video_format.fmt.pix.width);
        printf("height: %u\n", video_format.fmt.pix.height);
        printf("pixelformat: %s\n", get_pixel_format(video_format.fmt.pix.pixelformat, buffer));
        printf("bytesperline: %u\n", video_format.fmt.pix.bytesperline);
        printf("sizeimage: %u\n", video_format.fmt.pix.sizeimage);
        printf("colorspace: %u\n", video_format.fmt.pix.colorspace);
        printf("priv: 0x%08x\n", video_format.fmt.pix.priv);
        printf("flags: %u\n", video_format.fmt.pix.flags);
        printf("quantization: %u\n", video_format.fmt.pix.quantization);
        printf("xfer_func: %u\n", video_format.fmt.pix.xfer_func);

        close(fd);
    }
    else
    {
        printf("usage: video-info <device>\n");
    }

    return EXIT_SUCCESS;
}