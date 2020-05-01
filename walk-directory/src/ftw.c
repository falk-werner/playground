#include "strategies.h"

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stddef.h>

#define FTW_OPEN_FDS 20

static void * g_ftw_user_data = NULL;
static walk_directory_callback_fn * g_ftw_callback = NULL;

static int ftw_callback(
    char const * path,
    struct stat const * stat_buffer,
    int entry_type,
    struct FTW * ftwbuf)
{
    (void) stat_buffer;
    (void) ftwbuf;

    if ((FTW_F == entry_type) || (FTW_D == entry_type))
    {
        g_ftw_callback(g_ftw_user_data, (FTW_F == entry_type) ? FILETYPE_REG : FILETYPE_DIR, path);
    }

    return 0;
}

int
walk_directory_ftw(
    char const * base,
    walk_directory_callback_fn * callback,
    void * user_data)
{
    int result = 1;

    if (NULL == g_ftw_callback)
    {
        g_ftw_callback = callback;
        g_ftw_user_data = user_data;

        nftw(base, &ftw_callback, FTW_OPEN_FDS, FTW_PHYS);

        g_ftw_callback = NULL;
        g_ftw_user_data = NULL;
    }

    return result;
}
