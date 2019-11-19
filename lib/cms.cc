#include "openssl++/cms.hpp"
#include "openssl++/exception.hpp"
#include "openssl++/bio.hpp"
#include "openssl++/base64.hpp"

namespace openssl
{

CMS CMS::sign(X509 * cert, EVP_PKEY * key, STACK_OF(X509) * certs, BIO * data, unsigned int flags)
{
    CMS_ContentInfo * cms = CMS_sign(cert, key, certs, data, flags);
    if (NULL == cms)
    {
        throw OpenSSLException("unable to sign file");
    }

    return std::move(CMS(cms));
}

CMS::CMS(CMS_ContentInfo * cms_)
: cms(cms_)
{

}

CMS::CMS(CMS && other)
{
    this->cms = other.cms;
    other.cms = nullptr;
}

CMS & CMS::operator=(CMS && other)
{
    CMS_ContentInfo_free(this->cms);

    this->cms = other.cms;
    other.cms = nullptr;

    return *this;
}

CMS::~CMS()
{
    CMS_ContentInfo_free(cms);
}

CMS::operator CMS_ContentInfo*()
{
    return cms;
}

std::string CMS::toBase64() const
{
    auto signature = BasicIO::fromMemory();
    if (!i2d_CMS_bio(signature, cms))
    {
        throw OpenSSLException("unable to read signature");
    }

    char const * data;
    long count = BIO_get_mem_data(signature, &data);

    return Base64::encode(std::string(data, count));
}

void CMS::dump() const
{
    BIO * out = BIO_new_fd(fileno(stderr), BIO_NOCLOSE);
    CMS_ContentInfo_print_ctx(out,cms, 0, NULL);

    BIO_free_all(out);
}

}