#ifndef ITEM_LIST_H
#define ITEM_LIST_H

#include <menu.h>

#ifdef __cplusplus
extern "C"
{
#endif


extern ITEM * *
items_from_directory(
    char const * directory);

extern void
items_release(
    ITEM * * items);

#ifdef __cplusplus
}
#endif

#endif
