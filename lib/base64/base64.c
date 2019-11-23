#include "base64/base64.h"

size_t base64_encoded_size(size_t length)
{
    return ((length + 2) / 3) * 4;
}

extern char * base64_encode(
    uint8_t const * data,
    size_t length,
    char * buffer,
    size_t buffer_size)
{    
    static char const table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t const length_needed = base64_encoded_size(length);
    if (buffer_size < length_needed)
    {
        return NULL;
    }

    size_t pos = 0;
    size_t out_pos = 0;
    for(; (length - pos) >= 3; pos += 3)
    {
        buffer[out_pos++] = table[ data[pos] >> 2 ];
        buffer[out_pos++] = table[ ((data[pos    ] & 0x03) << 4) | (data[pos + 1] >> 4) ];
        buffer[out_pos++] = table[ ((data[pos + 1] & 0x0f) << 2) | (data[pos + 2] >> 6) ];
        buffer[out_pos++] = table[ data[pos + 2] & 0x3f ];
    }

    switch((length - pos))
    {
        case 1:
            buffer[out_pos++] = table[ data[pos] >> 2 ];
            buffer[out_pos++] = table[ ((data[pos] & 0x03) << 4) ];
            buffer[out_pos++] = '=';
            buffer[out_pos++] = '=';
            break;
        case 2:
            buffer[out_pos++] = table[ data[pos] >> 2 ];
            buffer[out_pos++] = table[ ((data[pos    ] & 0x03) << 4) | (data[pos + 1] >> 4) ];
            buffer[out_pos++] = table[ ((data[pos + 1] & 0x0f) << 2) ];
            buffer[out_pos++] = '=';
            break;
        default:
            break;
    }

    if (buffer_size >= out_pos)
    {
        buffer[out_pos] = '\0';
    }

    return buffer;
}

uint8_t * base64_decode(
    char const * data,
    size_t length,
    size_t * decoded_length)
{
    return NULL;
}
