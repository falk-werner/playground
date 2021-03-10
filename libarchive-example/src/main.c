#include <archive.h>
#include <archive_entry.h>
#include <getopt.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE (10 * 1024)

struct context
{
    char const * archive_file;
    char const * entry_name;
    const char * local_filename;
    bool verbose;
    bool show_help;
    int exit_code;
};

static void print_usage(void)
{
    printf(
        "(c) 2021 by Falk Werner <github.com/falk-werner>\n"
        "Extract a file from an archive\n"
        "\n"
        "Usage:\n"
        "\tarchive-example -a <archive> -e <entry> [-f <local-filename>] [-v]\n"
        "\n"
        "Options:\n"
        "\t-a, --archive - name of the archive file\n"
        "\t-e, --entry   - name of the entry to extract\n"
        "\t-f, --file    - name of the local file\n"
        "\t-v, --verbose - print additional informaiton\n"
        "\t-h, --help    - print this message\n"
    );
}

static void
parse_args(
    struct context * ctx,
    int argc,
    char * argv[])
{
    static struct option const long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"archive", required_argument, NULL, 'a'},
        {"entry", required_argument, NULL, 'e'},
        {"file", required_argument, NULL, 'f'},
        {"verbose", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };
    ctx->show_help = false;
    ctx->exit_code = EXIT_SUCCESS;
    ctx->archive_file = NULL;
    ctx->entry_name = NULL;
    ctx->local_filename = NULL;
    ctx->verbose = false;

    optind = 0;
    opterr = 0;
    bool finihed = false;
    while (!finihed)
    {
        int option_index = 0;
        int c = getopt_long(argc, argv, "a:e:f:vh", long_options, &option_index);
        switch (c)
        {
            case -1:
                finihed = true;
                break;
            case 'h':
                ctx->show_help = true;
                finihed = true;
                break;
            case 'a':
                ctx->archive_file = optarg;
                break;
            case 'e':
                ctx->entry_name = optarg;
                break;
            case 'f':
                ctx->local_filename = optarg;
                break;
            case 'v':
                ctx->verbose = true;
                break;
            default:
                fprintf(stderr, "error: invalid option\n");
                ctx->exit_code = EXIT_FAILURE;
                ctx->show_help = true;
                finihed = true;
                break;
        }
    }

    if ((!ctx->show_help) && (NULL == ctx->archive_file))
    {
        fprintf(stderr, "error: missing required argument: -a\n");
        ctx->exit_code = EXIT_FAILURE;
        ctx->show_help = true;
    }

    if ((!ctx->show_help) && (NULL == ctx->entry_name))
    {
        fprintf(stderr, "error: missing required argument: -e\n");
        ctx->exit_code = EXIT_FAILURE;
        ctx->show_help = true;
    }

    if ((!ctx->show_help) && (NULL == ctx->local_filename))
    {
        ctx->local_filename = ctx->entry_name;
    }

}

static int
extract_to(
    struct archive * archive,
    char const * filename)
{
    int exit_code = EXIT_FAILURE;

    FILE * file = fopen(filename, "wb");
    if (NULL != file)
    {
        void const * buffer;
        size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
    	int64_t offset;
#else
    	off_t offset;
#endif
        int rc = archive_read_data_block(archive, &buffer, &size, &offset);
        while (ARCHIVE_OK == rc)
        {
            fwrite(buffer, 1, size, file);
            rc = archive_read_data_block(archive, &buffer, &size, &offset);
        }

        fclose(file);
        exit_code = EXIT_SUCCESS;
    }

    return exit_code;
}


static int extract(struct context * ctx)
{
    struct archive * archive = archive_read_new();
    archive_read_support_format_tar(archive);
    archive_read_support_filter_all(archive);

    int rc = archive_read_open_filename(archive, ctx->archive_file, BLOCK_SIZE);
    if (0 != rc)
    {
        fprintf(stderr, "error: failed to open archive: %s\n", archive_error_string(archive));
        archive_read_free(archive);
        return EXIT_FAILURE;
    }

    struct archive_entry * entry;
    rc = archive_read_next_header(archive, &entry);
    while (ARCHIVE_OK == rc)
    {
        char const * name = archive_entry_pathname(entry);
        if (ctx->verbose)
        {
            printf("%s\n", name);
        }

        if (0 == strcmp(ctx->entry_name, name))
        {
            ctx->exit_code = extract_to(archive, ctx->local_filename);
        }
        rc = archive_read_next_header(archive, &entry);
    }

    archive_read_close(archive);
    archive_read_free(archive);
    return ctx->exit_code;
}

int main(int argc, char * argv[])
{
    struct context ctx;
    parse_args(&ctx, argc, argv);

    if (!ctx.show_help)
    {
        ctx.exit_code = extract(&ctx);
    }
    else
    {
        print_usage();
    }

    return ctx.exit_code;
}