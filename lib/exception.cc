#include "openssl++/exception.hpp"

#include <openssl/err.h>
#include <sstream>
#include <iomanip>

namespace
{

std::string getOpenSSLError(std::string const & message)
{
    unsigned long error_code = ERR_get_error();
    
    char const * lib_name = ERR_lib_error_string(error_code);
    char const * func_name = ERR_func_error_string(error_code);
    char const * reason = ERR_reason_error_string(error_code);

    std::stringstream stream;
    stream << "error: " << message
        << " (OpenSSL: " << lib_name << ": " << func_name << ": " << reason 
        << " [0x" << std::setw(8) << std::setfill('0') << std::hex << error_code << "])"
    ;

    return stream.str();
}

}

namespace openssl
{

OpenSSLBaseException::OpenSSLBaseException(std::string const & message_)
: message(message_)
{

}

OpenSSLBaseException::~OpenSSLBaseException()
{

}

char const * OpenSSLBaseException::what() const noexcept
{
    return message.c_str();
}

FileNotFoundException::FileNotFoundException(std::string const & filename_)
: OpenSSLBaseException("file not found: " + filename_)
, filename(filename_)
{

}

FileNotFoundException::~FileNotFoundException()
{

}

std::string const & FileNotFoundException::path() const
{
    return filename;
}

OpenSSLException::OpenSSLException(std::string const & message_)
: OpenSSLBaseException(getOpenSSLError(message_))
{

}

OpenSSLException::~OpenSSLException()
{

}

}