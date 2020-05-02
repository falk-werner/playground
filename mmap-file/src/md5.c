#include "md5.h"
#include <openssl/md5.h>
#include <stdlib.h>
#include <stdio.h>

#define MD5_SIZE 16

struct md5
{
    MD5_CTX context;
};


static char * 
md5_get_checksum(
    char * digest)
{
    char * checksum = malloc((2 * MD5_SIZE) + 1);
    int j = 0;
    for(int i = 0; i < MD5_SIZE; i++)
    {
        static char const hex[] = "0123456789abcdef";
        char low = digest[i] & 0x0f;
        char high = (digest[i] >> 4) & 0x0f;

        checksum[j++] = hex[high];
        checksum[j++] = hex[low];
    }
    checksum[j] = '\0';

    return checksum;
}

struct md5 *
md5_create(void)
{
    struct md5 * md5 = malloc(sizeof(struct md5));
    MD5_Init(&md5->context);

    return md5;
}

void
md5_update(
    struct md5 * md5,
    char const * data,
    size_t length)
{
    MD5_Update(&md5->context, data, length);
}

char *
md5_finalize(
    struct md5 * md5)
{
    char digest[MD5_SIZE];
    MD5_Final(digest, &md5->context);
    char * checksum = md5_get_checksum(digest);

    free(md5);
    return checksum;
}
