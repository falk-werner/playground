#define _GNU_SOURCE

#include "filewalker.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct context
{
    char const * directory;
    char const * pattern;
    int exit_code;
};

struct pattern
{
    char const * text;
    size_t size;
};

static void
print_usage(void)
{
    printf(
        "find-in-files, (c) 2020 Falk Werner\n"
        "Prints names of file containig a given text\n"
        "\n"
        "Usage:\n"
        "\tfind-in-files [-d <directory>] <text>\n"
        "\n"
        "Arguments:\n"
        "-d, --directory - search directory (default: .)\n"
        "<text>          - text to find (exact match)\n"
        "\n"
        "Example:\n"
        "\tfind-in-files brummni\n"
    );
}

static bool
parse_args(
    int argc,
    char * argv[],
    struct context * context)
{
    static struct option const options[] =
    {
        {"directory", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    context->directory = ".";
    context->pattern = NULL;
    context->exit_code = EXIT_SUCCESS;

    optind = 0;
    opterr = 0;
    bool is_finished = false;
    bool show_help = false;
    while (!is_finished)
    {
        int option_index = 0;
        char const c = getopt_long(argc, argv, "d:h", options, &option_index);
        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'd':
                context->directory = optarg;
                break;
            case 'h':
                show_help = true;
                is_finished = true;
                break;
            default:
                fprintf(stderr, "error: unknown argument\n");
                context->exit_code = EXIT_FAILURE;
                show_help = true;
                is_finished = true;
                break;
        }
    }

    if ((EXIT_SUCCESS == context->exit_code) && (!show_help))
    {
        if (optind < argc)
        {
            context->pattern = argv[optind];
        }
        else
        {
            fprintf(stderr, "error: missing text\n");
            context->exit_code = EXIT_FAILURE;
            show_help = true;
        }
    }

    if (show_help)
    {
        print_usage();
    }

    return ((EXIT_SUCCESS == context->exit_code) && (!show_help));
}

static void
find_in_file(
    void * user_data,
    int parent,
    char const * name,
    char const * full_name)
{
    struct pattern * pattern = user_data;

    int fd = openat(parent, name, O_RDONLY);
    if (0 <= fd)
    {
        struct stat stat_buffer;
        fstat(fd, &stat_buffer);
        size_t file_size = stat_buffer.st_size;

        char * buffer = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (NULL != buffer)
        {
            if (NULL != memmem(buffer, file_size, pattern->text, pattern->size))
            {
                puts(full_name);
            }

            munmap(buffer, file_size);
        }


        close(fd);
    }
}

int main(int argc, char * argv[])
{
    struct context context;
    if (parse_args(argc, argv, &context))
    {
        struct pattern pattern;
        pattern.text = context.pattern;
        pattern.size = strlen(context.pattern);

        filewalker_walk(context.directory, &find_in_file, &pattern);
    }

    return context.exit_code;
}