#include "download.h"

#include <atomic_ops.h>
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct download
{
    CURL * curl;
    char * url;
    char * filename;
    AO_t is_canceled;
};

static int download_progress(
    void * user_data,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t ultotal,
    curl_off_t ulnow)
{
    (void) dltotal;
    (void) dlnow;
    (void) ultotal;
    (void) ulnow;

    struct download * handle = user_data;
    AO_t is_canceled = AO_load(&handle->is_canceled);
    return (0 == is_canceled) ? 0 : 1;
}

struct download *
download_create(
    char const * url,
    char const * filename)
{
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        return NULL;
    }

    struct download * handle = malloc(sizeof(struct download));
    handle->curl = curl;
    handle->url = strdup(url);
    handle->filename = strdup(filename);
    handle->is_canceled = 0;

    return handle;
}

void
download_release(
    struct download * handle)
{
    if (NULL != handle)
    {
        free(handle->filename);
        free(handle->url);
        curl_easy_cleanup(handle->curl);
        free(handle);
    }
}

static int
download_perform_once(
    struct download * handle, FILE * file)
{
    long file_position = ftell(file);

    curl_easy_setopt(handle->curl, CURLOPT_URL, handle->url);
    curl_easy_setopt(handle->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle->curl, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(handle->curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(handle->curl, CURLOPT_XFERINFOFUNCTION, &download_progress);
    curl_easy_setopt(handle->curl, CURLOPT_XFERINFODATA, handle);
    curl_easy_setopt(handle->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle->curl, CURLOPT_RESUME_FROM_LARGE, file_position);

    CURLcode code = curl_easy_perform(handle->curl);

    long http_status = 500;
    if (code == CURLE_OK)
    {
        curl_easy_getinfo(handle->curl, CURLINFO_RESPONSE_CODE, &http_status);
    }

    return (int) http_status;
}

int
download_perform(
    struct download * handle)
{
    FILE * file = fopen(handle->filename, "wb");
    if (NULL == file)
    {
        return 500;
    }

    int http_status = 500;
    bool finished = false;
    int download_attempts = 5;
    while ((!finished) && (download_attempts > 0))
    {
        download_attempts--;
        http_status = download_perform_once(handle, file);
        finished = ((200 <= http_status) && (http_status < 300));
    }

    fclose(file);
    return http_status;
}

void
download_cancel(
    struct download * handle)
{
    AO_store(&handle->is_canceled, 1);
}
