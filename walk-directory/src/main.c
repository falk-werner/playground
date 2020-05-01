#include "strategies.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

struct context
{
    char const * strategy;
    char const * directory;
    int exit_code;
};

static void print_usage(void)
{
    printf(
        "walk-directory, (c) 2020 Falk Werner\n"
        "\n"
        "Usage:\n"
        "\twalk-directory [-s <strategy>] <directory>\n"
        "\n"
        "Arguments:\n"
        "\t-s, --strategy - specify traversal strategy, one of\n"
        "\t                 readdir - naive implementation based on readdir (default)\n"
        "\t                 ftw     - use ftw (see man 3 ftw)\n"
        "\t<directory>    - directory to walk\n"
        "\n"
        "Example:\n"
        "\twalk-directory .\n"
    );
}

static bool
parse_arguments(
    int argc,
    char * argv[],
    struct context * context)
{
    context->strategy = NULL;
    context->directory = NULL;
    context->exit_code = EXIT_SUCCESS;

    static struct option const options[] =
    {
        {"strategy", required_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
 
    optind = 0; // re-init getopt (useful during unit tests)
    opterr = 0; // prevent printing error messages by getopt
    bool show_help = false;
    bool is_finished = false;
    while (!is_finished)
    {
        int option_index = 0;
        char const c = getopt_long(argc, argv, "s:d:h", options, &option_index);
        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 's':
                context->strategy = optarg;
                break;
            case 'h':
                show_help = true;
                is_finished = true;
                break;
            default:
                fprintf(stderr, "error: invalid arguments\n");
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
            context->directory = argv[optind];
        }
        else
        {
            fprintf(stderr, "error: invalid arguments\n");
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

static int 
callback(
    void * user_data,
    int type,
    char const * name)
{
    (void) user_data;
    printf("%c %s\n", (FILETYPE_REG == type) ? 'F' : 'D' ,name);
}


int main(int argc, char* argv[])
{
    struct context context;
    if (parse_arguments(argc, argv, &context))
    {
        walk_directory_fn * walk = walk_directory_get_strategy(context.strategy);
        if (NULL != walk)
        {
            walk(context.directory, &callback, NULL);
        }
        else
        {
            fprintf(stderr, "error: unknown strategy\n");
            context.exit_code = EXIT_FAILURE;
        }
    }

    return context.exit_code;
}