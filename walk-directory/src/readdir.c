#include "strategies.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct dirname
{
    char * name;
    size_t length;
    size_t capacity;
};

static size_t max(size_t a, size_t b)
{
    return (a > b) ? a : b;
}

static void dirname_add(struct dirname * dirname, char const * path)
{
    size_t const length = strlen(path) + 1;
    size_t const needed = dirname->length + 1 + length;
    while (needed > dirname->capacity)
    {
        dirname->capacity *= 2;
        dirname->name = realloc(dirname->name, dirname->capacity);
    }

    dirname->name[dirname->length] = '/';
    memcpy(&(dirname->name[dirname->length + 1]), path, length);
    dirname->length += length;
}

static void walk(
    DIR * dir,
    struct dirname * dirname,
    walk_directory_callback_fn * callback,
    void * user_data)

{
    if (NULL != dir)
    {
        callback(user_data, FILETYPE_DIR, dirname->name);

        size_t const old_name_length = dirname->length;
        int fd = dirfd(dir);
        struct dirent * entry = readdir(dir);
        while (NULL != entry)
        {
            switch (entry->d_type)
            {
                case DT_DIR:
                    {
                        if ((0 != strcmp(entry->d_name, ".")) && (0 != strcmp(entry->d_name, "..")))
                        {
                            int subdir_fd = openat(fd, entry->d_name, O_DIRECTORY , O_RDONLY);
                            if (0 < fd)
                            {
                                dirname_add(dirname, entry->d_name);
                                
                                DIR * subdir = fdopendir(subdir_fd);
                                walk(subdir, dirname, callback, user_data);

                                dirname->length = old_name_length;
                                dirname->name[old_name_length] = '\0';
                            }
                        }
                    }
                    break;
                case DT_REG:
                    {
                        dirname_add(dirname, entry->d_name);

                        callback(user_data, FILETYPE_REG, dirname->name);

                        dirname->length = old_name_length;
                        dirname->name[old_name_length] = '\0';
                    }
                    break;
                default:
                    break;
            }

            entry = readdir(dir);
        }
        closedir(dir);
    }

}


int
walk_directory_readdir(
    char const * base,
    walk_directory_callback_fn * callback,
    void * user_data)
{
    struct dirname dirname;
    dirname.length = strlen(base);
    dirname.capacity = max(PATH_MAX, 2 * dirname.length);
    dirname.name = malloc(dirname.capacity);
    memcpy(dirname.name, base, dirname.length + 1);

    DIR * dir = opendir(base);
    walk(dir, &dirname, callback, user_data);

    free(dirname.name);

    return EXIT_SUCCESS;
}
