#include "openssl++/bio.hpp"
#include "openssl++/exception.hpp"

namespace openssl
{

BasicIO BasicIO::fromInputFile(std::string const & filename)
{
    BIO * file = BIO_new_file(filename.c_str(), "rb");
    if (NULL == file)
    {
        throw FileNotFoundException(filename);
    }

    return std::move(BasicIO(file));
}

BasicIO BasicIO::fromMemory()
{
    BIO * bio = BIO_new(BIO_s_mem());
    return std::move(BasicIO(bio));
}

BasicIO::BasicIO(BIO * bio_)
: bio(bio_)
{

}

BasicIO::~BasicIO()
{
    BIO_free_all(bio);
}

BasicIO & BasicIO::operator=(BasicIO && other)
{
    BIO_free_all(this->bio);

    this->bio = other.bio;
    other.bio = nullptr;

    return *this;
}

BasicIO::BasicIO(BasicIO && other)
{
    this->bio = other.bio;
    other.bio = nullptr;
}

BasicIO::operator BIO*()
{
    return bio;
}

}
