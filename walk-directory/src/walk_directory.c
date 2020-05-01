#include "strategies.h"
#include <stdlib.h>
#include <string.h>

walk_directory_fn *
walk_directory_get_strategy(
    char const * name)
{
    walk_directory_fn * result = NULL;

    if ((NULL == name) || (0 == strcmp(name, "readdir")))
    {
        result = &walk_directory_readdir;
    }
    else if (0 == strcmp(name, "ftw"))
    {
        result = &walk_directory_ftw;
    }
    
    return result;
}
