#include "strategy.h"
#include <string.h>

strategy_fn *
strategy_get(
    char const * name)
{
    strategy_fn * result = NULL;

    if (0 == strcmp("mmap", name))
    {
        result = &strategy_mmap;
    }
    else if (0 == strcmp("readall", name))
    {
        result = &strategy_readall;
    }
    else if (0 == strcmp("buffered", name))
    {
        result = &strategy_buffered;
    }

    return result;    
}
