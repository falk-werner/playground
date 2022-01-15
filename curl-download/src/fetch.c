#include "fetch.h"
#include <stdlib.h>

struct fetch_handle
{
    int status;
};

struct fetch_handle *
fetch(
    char const * url,
    char const * filename)
{
    struct fetch_handle * handle = malloc(sizeof(struct fetch_handle));
    handle->status = 500;
    return handle;
}

void
fetch_release(
    struct fetch_handle * handle)
{
    free(handle);
}

int
fetch_wait(
    struct fetch_handle * handle)
{
    return handle->status;
}

void
fetch_cancel(
    struct fetch_handle * handle)
{

}
