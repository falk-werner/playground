#include "video_stream.h"
#include "item_list.h"

#include <menu.h>
#include <linux/limits.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct context
{
    char const * device;
    char const * dir;
    int exit_code;
};

static void
print_usage(void)
{
    printf(
        "vmenu, (c) 2020 Falk Werner\n"
        "\n"
        "Usage:\n"
        "\tvmenu [-d <video-device>] -i <directory>\n"
        "\n"
        "Options:\n"
        "\t-d, --device    - path to video device (default: /dev/video0\n"
        "\t-i, --image-dir - path to image directory (default: .)\n"
        "\t-h, --help      - prints this message\n"
        "\n"
        "Example\n"
        "\tvmenu -d /dev/video0 -i img\n"
    );
}

static bool
parse_args(
    struct context * context,
    int argc,
    char * argv[])
{
    static struct option const options[] =
    {
        {"device"   , required_argument, NULL, 'd'},
        {"image-dir", required_argument, NULL, 'i'},
        {"help"     , no_argument      , NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    context->device = "/dev/video0";
    context->dir = ".";
    context->exit_code = EXIT_SUCCESS;
    bool show_help = false;

    optind = 0;
    opterr = 0;
    bool is_finished = false;
    while (!is_finished)
    {
        int option_index = 0;
        char const c = getopt_long(argc, argv, "d:i:h", options, &option_index);
        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'd':
                context->device = optarg;
                break;
            case 'i':
                context->dir = optarg;
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

    if (show_help)
    {
        print_usage();
    }

    return ((!show_help) && (EXIT_SUCCESS == context->exit_code));
}

static void
quit(void)
{
    endwin();
}

static int
run_menu(
    struct video_stream * stream,
    char const * directory)
{
    int result = EXIT_SUCCESS;

    ITEM ** items = items_from_directory(directory);
    if (NULL != items)
    {
        initscr();
        atexit(quit);
        clear();
        noecho();
        curs_set(0);
        cbreak();
        nl();
        timeout(500);
        keypad(stdscr, TRUE);


        WINDOW * win = newwin(LINES - 5, COLS - 2, 1, 1);
        box(win, 0 ,0);
        mvwaddstr(win, 0, 2, " Images ");

        mvaddstr(LINES - 2, 2, "Active: <none>");

        MENU * menu = new_menu(items);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, LINES - 7, COLS - 4, 2, 2));
        post_menu(menu);

        refresh();
        wrefresh(win);

        ITEM * current = items[0];
        bool is_running = true;
        while(is_running)
        {
            int key = getch();
            switch(key)
            {
                case KEY_DOWN:
                    menu_driver(menu, REQ_DOWN_ITEM);
                    wrefresh(win);
                    break;
                case KEY_UP:
                    menu_driver(menu, REQ_UP_ITEM);
                    wrefresh(win);
                    break;
                case '\n':
                     current = current_item(menu);
                    // fall-through
                case ERR:
                    {
                        char const * name = item_name(current);
                        char filename[PATH_MAX];
                        snprintf(filename, PATH_MAX, "%s/%s", directory, name);

                        char * error_message;
                        if (video_stream_put_image(stream, filename, &error_message))
                        {
                            mvprintw(LINES - 2, 2, "Active: %s", name);
                        }
                        else
                        {
                            mvprintw(LINES - 2, 2, "Error: %s", error_message);
                            free(error_message);
                        }
                        
                        clrtoeol();
                        refresh();
                        wrefresh(win);
                    }
                    break;
                case 27:
                    // fall-through
                case 'q':
                    // fall-through
                case KEY_F(10):
                    is_running = false;
                    break;
                default:
                    break;
            }
        }

        unpost_menu(menu);
        free_menu(menu);

        delwin(win);

        items_release(items);
    }
    else
    {
        fprintf(stderr, "error: failed to load files\n");
        result = EXIT_FAILURE;
    }

    return result;
}

int
main(
    int argc,
    char * argv[])
{
    struct context context;
    if (parse_args(&context, argc, argv))
    {
        struct video_stream * stream = video_stream_create(context.device);
        if (NULL != stream)
        {
            context.exit_code = run_menu(stream, context.dir);
            video_stream_close(stream);
        }
        else
        {
            fprintf(stderr, "error: failed to open video device: %s\n", context.device);
            context.exit_code = EXIT_FAILURE;
        }
        
    }

    return context.exit_code;
}