#include "filewalker.h"
#include "path.h"

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include <stddef.h>
#include <string.h>

static void
filewalker_walk_dir(
    int parent,
    char const * name,
    struct path * path,
    filewalker_callback_fn * callback,
    void * user_data)
{
    int fd = openat(parent, name, O_RDONLY);
    if (0 <= fd)
    {
        DIR * dir = fdopendir(fd);
        if (NULL != dir)
        {
            struct dirent * entry = readdir(dir);
            while (NULL != entry)
            {
                switch (entry->d_type)
                {
                    case DT_DIR:
                        if ((0 != strcmp(".", entry->d_name)) && (0 != strcmp("..", entry->d_name)))
                        {
                            int path_state = path_save(path);
                            path_add(path, entry->d_name);
                            filewalker_walk_dir(fd, entry->d_name, path, callback, user_data);
                            path_revert_to(path, path_state);
                        }
                        break;
                    case DT_REG:
                        {
                            int path_state = path_save(path);
                            path_add(path, entry->d_name);
                            callback(user_data, fd, entry->d_name, path_to_string(path));
                            path_revert_to(path, path_state);
                        }
                        break;
                    default:
                        break;
                }

                entry = readdir(dir);
            }
            closedir(dir);
        }
        else
        {
            close(fd);
        }        
    }
}

void
filewalker_walk(
    char const * base_directory,
    filewalker_callback_fn * callback,
    void * user_data)
{
    struct path path;
    path_init(&path, base_directory);

    filewalker_walk_dir(AT_FDCWD, base_directory, &path, callback, user_data);
    path_cleanup(&path);
}
