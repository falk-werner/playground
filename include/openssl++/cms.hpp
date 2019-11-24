/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef OPENSSL_CMS_HPP
#define OPENSSL_CMS_HPP

#include <openssl/cms.h>
#include <string>

namespace openssl
{

class CMS
{
    CMS & operator=(CMS const &) = delete;
    CMS(CMS const &) = delete;
public:
    static CMS fromFile(std::string const & filename);
    static CMS sign(X509 * cert, EVP_PKEY * key, STACK_OF(X509) * certs, BIO * data, unsigned int flags);
    explicit CMS(CMS_ContentInfo * cms_);
    CMS(CMS && other);
    CMS & operator=(CMS && other);
    ~CMS();
    operator CMS_ContentInfo*();
    void saveToFile(std::string const & filename) const;
    void saveToBIO(BIO * bio) const;
    void dump() const;
private:
    CMS_ContentInfo * cms;
};

}

#endif
