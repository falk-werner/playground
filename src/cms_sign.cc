/*
 * Copyright (c) 2019 Falk Werner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//#############################################################################
// Header
//#############################################################################

#include <iostream>
#include <cstdlib>

#include <getopt.h>

#include "openssl++/openssl++.hpp"


using openssl::OpenSSL;
using openssl::PrivateKey;
using openssl::BasicIO;
using openssl::Certificate;
using openssl::CMS;

//#############################################################################
// Type Definitions
//#############################################################################

/// Commnd, the application can execute.
enum command
{
    SIGN,               ///< sign file
    PRINT_USAGE         ///< print usage and exit
};

/// Context of the application.
///
/// Contains parsed command line arguments.
struct context
{
    char const * filename;      ///< name of the file to sign
    char const * key_file;      ///< name of the private key file (PEM)
    char const * cert_file;     ///< name of the certificate file used to sign (PEM)
    enum command command;       ///< command to execute
    bool is_verbose;            ///< if true, additional output will be printed to stderr
};


//#############################################################################
// Local Function Declarations
//#############################################################################

//-----------------------------------------------------------------------------
/// Prints the usage of the application.
//-----------------------------------------------------------------------------
static void print_usage(void);

//-----------------------------------------------------------------------------
/// Parses command line arguments.
///
/// \param context context of the application; used to store parsed arguemnts
/// \param argc    count of command line arguments
/// \param argv    command line arguments
///
/// \return EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
///         Note that context will be also initialized on failure.
//-----------------------------------------------------------------------------
static int parse_arguments(
    struct context * context,
    int argc,
    char * argv[]
);

//-----------------------------------------------------------------------------
/// Create a signature of a file and prints it to stdout.
///
/// \param filname   file to sign
/// \param key_file  PEM file containing private key
/// \param cert_file PEM file containing certificate used for signing
///
/// \return EXIT_SUCCESS on success, EXIT_FAILURE otherwise 
//-----------------------------------------------------------------------------
static int sign(
    char const * filename,
    char const * key_file,
    char const * cert_file,
    bool is_verbose);

//#############################################################################
// Implementation
//#############################################################################

//-----------------------------------------------------------------------------
/// Entry point of the application.
///
/// \param argc count of command line arguments
/// \param argv command line arguments
///
/// \return EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
//-----------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    OpenSSL::init();

    struct context context;
    int result = parse_arguments(&context, argc, argv);
    switch (context.command)
    {
        case SIGN:
            result = sign(context.filename, context.key_file, context.cert_file, context.is_verbose);
            break;
        case PRINT_USAGE:
            print_usage();
            break;
        default:
            std::cerr <<  "error: invalid command (" << context.command << "d)\n";
            break;
    }

    return result;
}

//-----------------------------------------------------------------------------
static void print_usage(void)
{
    std::cout <<
        "cms_sign, Copyright (c) 2019 Falk Werner\n"
        "Prints signature of a file using OpenSSL Cryptographic Message Syntax (CMS)\n"
        "\n"
        "Usage:\n"
        "\tcms_sign -f <infile> -p <private-key.pem> -c <certifate.pem> [-v] | -h\n"
        "\n"
        "Options:\n"
        "\t-f, --file        - file to sign\n"
        "\t-p, --private-key - private key to sign (format: PEM)\n"
        "\t-c, --certificate - certifiacte to sign (format: PEM)\n"
        "\t-v, --verbose     - print additional information to stderr\n"
        "\t-h, --help        - print usage\n"
        "\n"
        "OpenSSL: create self-signed certificate and key\n"
        "\topenssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes\n"
        "\n"
        "Examples:\n"
        "\tcms_sign -f any.file -p key.pem -c cert.pem\n"
    ;
}

//-----------------------------------------------------------------------------
static int parse_arguments(
    struct context * context,
    int argc,
    char * argv[]
)
{
    static struct option const options[] =
    {
        {"file", required_argument, NULL, 'f'},
        {"private-key", required_argument, NULL, 'p'},
        {"certificate", required_argument, NULL, 'c'},
        {"verbose", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int result = EXIT_SUCCESS;
    context->command = SIGN;
    context->filename = NULL;
    context->key_file = NULL;
    context->cert_file = NULL;
    context->is_verbose = false;

    bool is_finished = false;
    while (!is_finished)
    {
        int option_index = 0;
        int const c = getopt_long(argc, argv, "f:p:c:vh", options, &option_index);

        switch (c)
        {
            case -1:
                is_finished = true;
                break;
            case 'f':
                context->filename = optarg;
                break;
            case 'p':
                context->key_file = optarg;
                break;
            case 'c':
                context->cert_file = optarg;
                break;
            case 'v':
                context->is_verbose = true;
                break;
            case 'h':
                context->command = PRINT_USAGE;
                break;
            default:
                std::cout << "error: unknown argument\n";
                context->command = PRINT_USAGE;
                result = EXIT_FAILURE;
                is_finished = true;
                break;
        }
    }

    if (SIGN == context->command)
    {
        if (NULL == context->filename)
        {
            std::cerr <<  "error: missing required argument: file name (-f)\n";
            context->command = PRINT_USAGE;
            result = EXIT_FAILURE;
        }

        if (NULL == context->key_file)
        {
            std::cerr << "error: missing required argument: private key (-p)\n";
            context->command = PRINT_USAGE;
            result = EXIT_FAILURE;
        }

        if (NULL == context->cert_file)
        {
            std::cerr << "error: missing required argument: certificate (-c)\n";
            context->command = PRINT_USAGE;
            result = EXIT_FAILURE;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
static int sign(
    char const * filename,
    char const * key_file,
    char const * cert_file,
    bool is_verbose)
{
    int result = EXIT_FAILURE;

    try
    {
        auto key = PrivateKey::fromPEM(key_file);
        auto cert = Certificate::fromPEM(cert_file);
        auto file = BasicIO::fromInputFile(filename);

        auto cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);
        auto signature = cms.toBase64();
        std::cout << signature << std::endl;

        if (is_verbose)
        {
            cms.dump();
        }

        result = EXIT_SUCCESS;
    }
    catch(std::exception const& ex)
    {
        std::cerr << ex.what() << std::endl;;
    }
    
    return result;
}





