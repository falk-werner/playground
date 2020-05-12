#include "put_image.h"

#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct context
{
    char const * device;
    char const * filename;
    int exit_code;
};


static void
print_usage(void)
{
    printf(
        "put-image, (c) 2020 Falk Werner\n"
        "Writes an image to video device\n"
        "\n"
        "Usage:\n"
        "\tput-image [-d <video-device] -f <image.png>\n"
        "\n"
        "Options:\n"
        "\t-d, --device  - path of video device (default: /dev/video0)\n"
        "\t-f, --file    - path of image (must be PNG file)\n"
        "\t-h, --help    - prints this message\n"
        "\n"
        "Example:\n"
        "\tput-image -d /dev/video0 -f sleepy.png\n"
    );
}

static bool parse_args(
    struct context * context,
    int argc,
    char * argv[])
{
    static struct option const options[] =
    {
        {"device", required_argument, NULL, 'd'},
        {"file"  , required_argument, NULL, 'f'},
        {"help"  , no_argument      , NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    context->device = "/dev/video0";
    context->filename = NULL;
    context->exit_code = EXIT_SUCCESS;
    bool show_help = false;

    optind = 0;
    opterr = 0;
    bool is_finished = false;
    while (!is_finished)
    {
        int option_index = 0;
        char const c = getopt_long(argc, argv, "d:f:h", options, &option_index);
        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'd':
                context->device = optarg;
                break;
            case 'f':
                context->filename = optarg;
                break;
            case 'h':
                show_help = true;
                is_finished = true;
                break;
            default:
                fprintf(stderr, "error: unknown option\n");
                show_help = true;
                context->exit_code = EXIT_FAILURE;
                is_finished = true;
                break;
        }
    }

    if ((!show_help) && (EXIT_SUCCESS == context->exit_code))
    {
        if (NULL == context->filename)
        {
            fprintf(stderr, "error: missing filename\n");
            show_help = true;
            context->exit_code = EXIT_FAILURE;
        }
    }

    if (show_help)
    {
        print_usage();
    }

    return ((!show_help) && (EXIT_SUCCESS == context->exit_code));
}

int 
main(
    int argc,
    char * argv[])
{   
    struct context context;
    if (parse_args(&context, argc, argv))
    {
        char * error_message;
        if (!put_image(context.device, context.filename, &error_message))
        {
            fprintf(stderr, "error: %s\n", error_message);
            free(error_message);
            context.exit_code = EXIT_FAILURE;
        }
    }

    return context.exit_code;
}