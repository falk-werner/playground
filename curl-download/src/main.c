#include "fetch.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    int result = EXIT_FAILURE;

    if (3 == argc)
    {
        char const * url = argv[1];
        char const * filename = argv[2];
        struct fetch_handle * handle = fetch(url, filename);
        int status = fetch_wait(handle);
        fetch_release(handle);
        result = ((200 <= status) && (status < 300)) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else
    {
        printf("usage: download <url> <filename>\n");        
    }

    return result;
}