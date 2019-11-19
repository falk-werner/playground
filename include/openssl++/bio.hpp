#ifndef OPENSSL_BIO_HPP
#define OPENSSL_BIO_HPP

#include <openssl/bio.h>
#include <string>

namespace openssl
{

class BasicIO
{
    BasicIO operator=(BasicIO const &) = delete;
    BasicIO(BasicIO const &) = delete;
public:
    static BasicIO fromInputFile(std::string const & filename);
    static BasicIO fromMemory();
    explicit BasicIO(BIO * bio_);
    BasicIO & operator=(BasicIO && other);
    BasicIO(BasicIO && other);
    ~BasicIO();
    operator BIO*();
private:
    BIO * bio;
};

}

#endif
