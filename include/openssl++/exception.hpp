#ifndef OPENSSL_EXCEPTION_HPP
#define OPENSSL_EXCEPTION_HPP

#include <exception>
#include <string>

namespace openssl
{

class OpenSSLBaseException: public std::exception
{
public:
    OpenSSLBaseException(std::string const & message_);
    ~OpenSSLBaseException();
    char const * what() const noexcept override;
private:
    std::string message;
};

class FileNotFoundException: public OpenSSLBaseException
{
public:
    FileNotFoundException(std::string const & filename_);
    ~FileNotFoundException();
    std::string const & path() const;
private:
    std::string filename;
};

class OpenSSLException: public OpenSSLBaseException
{
public:
    OpenSSLException(std::string const & message_);
    ~OpenSSLException();
};

}

#endif
