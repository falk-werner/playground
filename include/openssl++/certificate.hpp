#ifndef OPENSSL_CERTIFICATE_HPP
#define OPENSSL_CERTIFICATE_HPP

#include <openssl/x509.h>
#include <string>

namespace openssl
{

class Certificate
{
    Certificate operator=(Certificate const &) = delete;
    Certificate(Certificate const &) = delete;
public:
    static Certificate fromPEM(std::string const & filename);
    explicit Certificate(X509 * cert_);
    Certificate & operator=(Certificate && other);
    Certificate(Certificate && other);
    ~Certificate();
    operator X509*();
private:
    X509 * cert;
};

}

#endif
