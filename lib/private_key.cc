#include "openssl++/private_key.hpp"
#include "openssl++/bio.hpp"
#include "openssl++/exception.hpp"

#include <openssl/pem.h>

namespace openssl
{

PrivateKey PrivateKey::fromPEM(std::string const & filename)
{
    BasicIO key_file = BasicIO::fromInputFile(filename);

    EVP_PKEY * key = PEM_read_bio_PrivateKey(key_file, NULL, NULL, NULL);
    if (NULL == key)
    {
        throw OpenSSLException("failed to parse key file");
    }

    return std::move(PrivateKey(key));
}

PrivateKey::PrivateKey(EVP_PKEY * key_)
: key(key_)
{

}

PrivateKey & PrivateKey::operator=(PrivateKey && other)
{
    EVP_PKEY_free(this->key);

    this->key = other.key;
    other.key = nullptr;

    return *this;
}

PrivateKey::PrivateKey(PrivateKey && other)
{
    this->key = other.key;
    other.key = nullptr;
}

PrivateKey::~PrivateKey()
{
    EVP_PKEY_free(key);
}

PrivateKey::operator EVP_PKEY *()
{
    return key;
}

}