#include "openssl++/cms.hpp"
#include "openssl++/exception.hpp"
#include "openssl++/basic_io.hpp"

#include <iostream>

namespace openssl
{

CMS CMS::fromFile(std::string const & filename)
{
    BasicIO file = BasicIO::openInputFile(filename);
    CMS_ContentInfo * cms = d2i_CMS_bio(file,NULL);
    if (NULL == cms)
    {
        throw new OpenSSLException("failed to read file");
    }

    return std::move(CMS(cms));
}

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

void CMS::saveToFile(std::string const & filename) const
{
    auto file = BasicIO::openOutputFile(filename);
    saveToBIO(file);
}

void CMS::saveToBIO(BIO * bio) const
{
    if (!i2d_CMS_bio(bio, cms))
    {
        throw OpenSSLException("failed to convert CMS to DER format");
    }
}


void CMS::dump() const
{
    BIO * out = BIO_new_fd(fileno(stderr), BIO_NOCLOSE);
    CMS_ContentInfo_print_ctx(out,cms, 0, NULL);

    BIO_free_all(out);
}

}