#include "openssl++/base64.hpp"

#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

namespace openssl
{

std::string Base64::encode(std::string const &data)
{
    BIO * base64 = BIO_new(BIO_f_base64());
    BIO * bio = BIO_new(BIO_s_mem());
    bio = BIO_push(base64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, data.c_str(), data.size());
    BIO_flush(bio);
    
    BUF_MEM * buffer;
    BIO_get_mem_ptr(bio, &buffer);
    std::string result(buffer->data, buffer->length);

    BIO_free_all(bio);
    return result;
}

}