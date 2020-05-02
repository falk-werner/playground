#include "strategy.h"
#include "md5.h"

#include <stdio.h>

#define BUFFER_SIZE (100 * 1024)

char *
strategy_buffered(
    char const * filename)
{
    char * checksum = NULL;
    FILE * file = fopen(filename, "rb");
    if (NULL != file)
    {
        struct md5 * md5 = md5_create();
        char buffer[BUFFER_SIZE];
        size_t bytes_read;
        do {
            bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
            md5_update(md5, buffer, bytes_read);
        } while (bytes_read == BUFFER_SIZE);

        checksum = md5_finalize(md5);
        fclose(file);
    }

    return checksum;
}
