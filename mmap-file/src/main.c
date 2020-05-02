#include "strategy.h"

#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct context
{
    strategy_fn * compute_checksum;
    char ** files;
    int file_count;
    int exit_code;
};

static void print_usage(void)
{
    printf(
        "checksum, (c) 2020 Falk Werner\n"
        "Prints MD5 checksum of one ore more files\n"
        "\n"
        "Usage:\n"
        "\tchecksum [-s <strategy>] <filename>...\n"
        "\n"
        "Arguments:\n"
        "-s, --strategy - strategy to scan file\n"
        "                 mmap     - using mmap (default)\n"
        "                 readall  - using read to load whole file at once\n"
        "                 buffered - using a 100KBye buffer\n"
        "\n"
        "Example:\n"
        "\tchecksum -s mmap some.file\n"

    );
}

static bool parse_args(
    int argc,
    char * argv[],
    struct context * context)
{
    static struct option const options[] =
    {
        {"strategy", required_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    context->compute_checksum = &strategy_mmap;
    context->files = NULL;
    context->file_count = 0;
    context->exit_code = EXIT_SUCCESS;

    optind = 0;
    opterr = 0;
    bool is_finished = false;
    bool show_help = false;
    while (!is_finished)
    {
        int option_index = 0;
        char const c = getopt_long(argc, argv, "s:h", options, &option_index);
        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 's':
                {
                    context->compute_checksum = strategy_get(optarg);
                    if (NULL == context->compute_checksum)
                    {
                        fprintf(stderr, "error: unknown strategy\n");
                        context->exit_code = EXIT_FAILURE;
                        show_help = true;
                        is_finished = true;
                    }
                }
                break;
            case 'h':
                show_help = true;
                is_finished = true;
                break;
            default:
                fprintf(stderr, "error: unknown argument\n");
                show_help = true;
                context->exit_code = EXIT_FAILURE;
                is_finished = true;
                break;
        }
    }

    if ((EXIT_SUCCESS == context->exit_code) && (!show_help))
    {
        if (optind < argc)
        {
            context->files = &(argv[optind]);
            context->file_count = argc - optind;
        }
        else
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

    return ((EXIT_SUCCESS == context->exit_code) && (!show_help));
}

int main(int argc, char * argv[])
{
    struct context context;
    if (parse_args(argc, argv, &context))
    {
        for (int i = 0; i < context.file_count; i++)
        {
            char const * filename = context.files[i];
            char * checksum = context.compute_checksum(filename);
            if (NULL != checksum)
            {
                printf("%s %s\n", checksum, filename);
                free(checksum);
            }
            else
            {
                fprintf(stderr, "error: failed to compute checksum of %s\n", filename);
            }            
        }
    }

    return context.exit_code;
}