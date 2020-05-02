#include "strategy.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>

char *
strategy_readall(
    char const * filename)
{
    char * checksum = NULL;
    FILE * file = fopen(filename, "rb");
    if (NULL != file)
    {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char * buffer = malloc(file_size);
        size_t bytes_read = fread(buffer, 1, file_size, file);
        if (bytes_read == file_size)
        {
            struct md5 * md5 = md5_create();
            md5_update(md5, buffer, file_size);
            checksum = md5_finalize(md5);
        }

        free(buffer);
        fclose(file);
    }

    return checksum;
}
