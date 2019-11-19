#ifndef OPENSSL_PRIVATE_KEY_HPP
#define OPENSSL_PRIVATE_KEY_HPP

#include <openssl/evp.h>
#include <string>

namespace openssl
{

class PrivateKey
{
    PrivateKey& operator=(PrivateKey const &) = delete;
    PrivateKey(PrivateKey const &) = delete;
public:
    static PrivateKey fromPEM(std::string const & filename);
    explicit PrivateKey(EVP_PKEY * key_);
    PrivateKey & operator=(PrivateKey && other);
    PrivateKey(PrivateKey && other);
    ~PrivateKey();
    operator EVP_PKEY*();
private:
    EVP_PKEY * key;
};

}



#endif
