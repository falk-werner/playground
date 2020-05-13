#include "item_list.h"

#include <menu.h>

#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ITEMS_DEFAULT_CAPACITY 16

struct item_list
{
    ITEM * * items;
    size_t size;
    size_t capacity;
};

static void
items_add(
    struct item_list * list,
    char const * name)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->items = realloc(list->items, sizeof(ITEM*) * list->capacity);
    }

    if (NULL != name)
    {
        list->items[list->size] = new_item(strdup(name), "");
    }
    else
    {
        list->items[list->size] = NULL;
    }
    
    list->size++;
}

static bool
ends_with(
    char const * value,
    char const * suffix)
{
    size_t value_size = strlen(value);
    size_t suffix_size = strlen(suffix);

    return ((value_size >= suffix_size) && 
        (0 == strcmp(&value[value_size - suffix_size],suffix)));
}

static bool
is_image(
    char const * filename)
{
    return (ends_with(filename, ".png"));
}

ITEM * *
items_from_directory(
    char const * directory)
{
    struct item_list list = { NULL, 0 , 0};

    DIR * dir = opendir(directory);
    if (NULL != dir)
    {
        list.capacity = ITEMS_DEFAULT_CAPACITY;
        list.size = 0;
        list.items = malloc(sizeof(ITEM*) * list.capacity);

        struct dirent * entry = readdir(dir);
        while (NULL != entry)
        {
            if ((DT_REG == entry->d_type) && (is_image(entry->d_name)))
            {
                items_add(&list, entry->d_name);
            }

            entry = readdir(dir);
        }

        if (0 < list.size)
        {
            items_add(&list, NULL);
        }
        else
        {
            free(list.items);
            list.items = NULL;
        }

        closedir(dir);
    }

    return list.items;
}

void
items_release(
    ITEM * * items)
{
    for(size_t i = 0; NULL != items[i]; i++)
    {
        char * name = (char*) item_name(items[i]);
        free_item(items[i]);
        free(name);
    }
    free(items);

}
