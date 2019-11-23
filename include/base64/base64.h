#ifndef BASE64_H
#define BASE64_H

#ifndef __cplusplus
#include <inttypes.h>
#include <stddef.h>
#else
#include <cinttypes>
#include <cstddef>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern size_t base64_encoded_size(size_t length);

extern char * base64_encode(
    uint8_t const * data,
    size_t length,
    char * buffer,
    size_t buffer_size);

extern uint8_t * base64_decode(
    char const * data,
    size_t length,
    size_t * decoded_length);

#ifdef __cplusplus
}
#endif

#endif
