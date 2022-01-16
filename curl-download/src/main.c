#include "download.h"

#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

struct download * g_handle = NULL;

static void handle_sigint(int signal_id)
{
    (void) signal_id;
    if (NULL != g_handle)
    {
        download_cancel(g_handle);
    }
}

int main(int argc, char * argv[])
{
    int result = EXIT_FAILURE;

    if (3 == argc)
    {
        curl_global_init(CURL_GLOBAL_ALL);

        signal(SIGINT, &handle_sigint);

        char const * url = argv[1];
        char const * filename = argv[2];
        g_handle = download_create(url, filename);
        int status = download_perform(g_handle);
        download_release(g_handle);
        result = ((200 <= status) && (status < 300)) ? EXIT_SUCCESS : EXIT_FAILURE;

        curl_global_cleanup();
    }
    else
    {
        printf("usage: fetch <url> <filename>\n");        
    }

    return result;
}