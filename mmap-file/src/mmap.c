#include "strategy.h"
#include "md5.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


extern char *
strategy_mmap(
    char const * filename)
{
    char * checksum = NULL;
    int fd = open(filename, O_RDONLY);
    if (0 <= fd)
    {
        struct stat stat_buffer;
        fstat(fd, &stat_buffer);
        size_t file_size = stat_buffer.st_size;

        char * buffer = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (NULL != buffer)
        {
            struct md5 * md5 = md5_create();
            md5_update(md5, buffer, file_size);
            checksum = md5_finalize(md5);

            munmap(buffer, file_size);
        }

        close(fd);
    }

    return checksum;
}
