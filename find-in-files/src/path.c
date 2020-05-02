#include "path.h"

#include <linux/limits.h>

#include <stdlib.h>
#include <string.h>

static size_t
path_max(
    size_t a,
    size_t b)
{
    return (a > b) ? a : b;
}

void
path_init(
    struct path * path,
    char const * name)
{
    size_t const name_size = strlen(name);
    path->size = name_size;
    path->capacity = path_max(PATH_MAX, 2 * name_size);
    path->name = malloc(path->capacity);
    memcpy(path->name, name, name_size + 1);
}

void
path_cleanup(
    struct path * path)
{
    free(path->name);
}

void
path_add(
    struct path * path,
    char const * name)
{
    size_t const name_size = strlen(name) + 1;
    size_t const needed = path->size + 1 + name_size;
    while (needed > path->capacity)
    {
        path->capacity *= 2;
        path->name = realloc(path->name, path->capacity);
    }

    path->name[path->size] = '/';
    memcpy(&(path->name[path->size + 1]), name, name_size);
    path->size += name_size;
}

char const *
path_to_string(
    struct path * path)
{
    return path->name;
}

int
path_save(
    struct path * path)
{
    return path->size;
}

void
path_revert_to(
    struct path * path,
    int old_state)
{
    path->name[old_state] = '\0';
    path->size = old_state;
}
