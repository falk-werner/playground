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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <getopt.h>

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/cms.h>

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
};


//#############################################################################
// Local Function Declarations
//#############################################################################

//-----------------------------------------------------------------------------
/// Initialized OpenSSL library.
///
/// \ņote The application will be terminated, if initialization fails.
//-----------------------------------------------------------------------------
static void init_openssl(void);

//-----------------------------------------------------------------------------
/// Prints an OpenSSL error message.
///
/// \param message user defined message to specify context of the error
//-----------------------------------------------------------------------------
static void print_openssl_error(
    char const * message);

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
    char const * cert_file);

//-----------------------------------------------------------------------------
/// Loads a private key from a PEM file.
///
/// \param filename name of PEM file containing the private key
///
/// \return Private key on success, NULL otherwise.
//-----------------------------------------------------------------------------
static EVP_PKEY *
read_key_from_file(
    char const * filename);

//-----------------------------------------------------------------------------
/// Loads a X509 certificate from a PEM file.
///
/// \param filename name of the PEM file containing the X509 certificate.
///
/// \return X509 certificate on success, NULL otherwise.
//-----------------------------------------------------------------------------
static X509 *
read_cert_from_file(
    char const * filename);

//-----------------------------------------------------------------------------
/// Encodes a block of given data in Base64 format.
///
/// \note The result must be freed by user.
///
/// \param data  Pointer to data block
/// \param count Length of data block in bytes.
///
/// \return base64 encoded data
//-----------------------------------------------------------------------------
static char *
base64_encode(
    char const * data, 
    size_t count);

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
    init_openssl();

    struct context context;
    int result = parse_arguments(&context, argc, argv);
    switch (context.command)
    {
        case SIGN:
            result = sign(context.filename, context.key_file, context.cert_file);
            break;
        case PRINT_USAGE:
            print_usage();
            break;
        default:
            fprintf(stderr, "error: invalid command (%d)\n", context.command);
            break;
    }

    return result;
}

//-----------------------------------------------------------------------------
static void init_openssl(void)
{
#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
	OPENSSL_config(NULL);
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
#else
    int const rc = OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    if (1 != rc)
    {
        print_openssl_error("unable to initialize openssl");

        exit(EXIT_FAILURE);
    }
#endif   
}

//-----------------------------------------------------------------------------
static void print_openssl_error(char const * message)
{
    unsigned long error_code = ERR_get_error();
    
    char const * lib_name = ERR_lib_error_string(error_code);
    char const * func_name = ERR_func_error_string(error_code);
    char const * reason = ERR_reason_error_string(error_code);

    fprintf(stderr, "error: %s (OpenSSL: %s: %s: %s [0x%08lX])\n", message, lib_name, func_name, reason, error_code);
}

//-----------------------------------------------------------------------------
static void print_usage(void)
{
    printf(
        "cms_sign, Copyright (c) 2019 Falk Werner\n"
        "Prints signature of a file using OpenSSL Cryptographic Message Syntax (CMS)\n"
        "\n"
        "Usage:\n"
        "\tcms_sign -f <infile> -p <private-key.pem> -c <certifate.pem> | -h\n"
        "\n"
        "Options:\n"
        "\t-f, --file        - file to sign\n"
        "\t-p, --private-key - private key to sign (format: PEM)\n"
        "\t-c, --certificate - certifiacte to sign (format: PEM)\n"
        "\t-h, --help        - print usage\n"
        "\n"
        "OpenSSL: create self-signed certificate and key\n"
        "\topenssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes\n"
        "\n"
        "Examples:\n"
        "\tcms_sign -f any.file -p key.pem -c cert.pem\n"
    );
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
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int result = EXIT_SUCCESS;
    context->command = SIGN;
    context->filename = NULL;
    context->key_file = NULL;
    context->cert_file = NULL;

    bool is_finished = false;
    while (!is_finished)
    {
        int option_index = 0;
        int const c = getopt_long(argc, argv, "f:p:c:h", options, &option_index);

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
            case 'h':
                context->command = PRINT_USAGE;
                break;
            default:
                fprintf(stderr, "error: unknown argument\n");
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
            fprintf(stderr, "error: missing required argument: file name (-f)\n");
            context->command = PRINT_USAGE;
            result = EXIT_FAILURE;
        }

        if (NULL == context->key_file)
        {
            fprintf(stderr, "error: missing required argument: private key (-p)\n");
            context->command = PRINT_USAGE;
            result = EXIT_FAILURE;
        }

        if (NULL == context->cert_file)
        {
            fprintf(stderr, "error: missing required argument: certificate (-c)\n");
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
    char const * cert_file)
{
    int result = EXIT_SUCCESS;

    EVP_PKEY * key = read_key_from_file(key_file);
    if (NULL == key)
    {
        result = EXIT_FAILURE; 
    }

    X509 * cert = NULL;
    if (EXIT_SUCCESS == result)
    {
        cert = read_cert_from_file(cert_file);
        if (NULL == cert)
        {
            result = EXIT_FAILURE;
        }
    }

    BIO * file = NULL;
    if (EXIT_SUCCESS == result)
    {
        file = BIO_new_file(filename, "rb");
        if (NULL == file)
        {
            fprintf(stderr, "error: cannot read file: \'%s\'\n", filename);
            result = EXIT_FAILURE;
        }
    }

    CMS_ContentInfo * cms = NULL;
    if (EXIT_SUCCESS == result)
    {
        cms = CMS_sign(cert, key, NULL, file, CMS_DETACHED | CMS_BINARY);
        if (NULL == cms)
        {
            print_openssl_error("unable to sign file");
            result = EXIT_FAILURE;
        }
    }

    if (EXIT_SUCCESS == result)
    {
        BIO * signature = BIO_new(BIO_s_mem());
        if (i2d_CMS_bio(signature, cms))
        {
            char const * data;
            long count = BIO_get_mem_data(signature, &data);

            char * base64 = base64_encode(data, count);
            puts(base64);
            free(base64);
        }
        else
        {
            print_openssl_error("unable to read signature");
            result = EXIT_FAILURE;
        }

        BIO_free_all(signature);
    }

    CMS_ContentInfo_free(cms);
    BIO_free_all(file);
    X509_free(cert);
    EVP_PKEY_free(key);

    return EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
static EVP_PKEY *
read_key_from_file(
    char const * filename)
{
    BIO * key_file = BIO_new_file(filename, "rb");
    if (NULL == key_file)
    {
        fprintf(stderr, "error: cannot open key file: \'%s\'\n", filename);
        return NULL;
    }

    EVP_PKEY * key = PEM_read_bio_PrivateKey(key_file, NULL, NULL, NULL);
    if (NULL == key)
    {
        print_openssl_error("failed to parse key file");
    }

    BIO_free_all(key_file);
    return key;
}

//-----------------------------------------------------------------------------
static X509 *
read_cert_from_file(
    char const * filename)
{
    BIO * cert_file = BIO_new_file(filename, "rb");
    if (NULL == cert_file)
    {
        fprintf(stderr, "error: cannot open certificate file: \'%s\'\n", filename);
        return NULL;
    }

    X509 * cert = PEM_read_bio_X509(cert_file, NULL, NULL, NULL);
    if (NULL == cert)
    {
        print_openssl_error("failed to parse certificate file");
    }

    BIO_free_all(cert_file);
    return cert;
}

//-----------------------------------------------------------------------------
static char *
base64_encode(
    char const * data, 
    size_t count)
{
    BIO * base64 = BIO_new(BIO_f_base64());
    BIO * bio = BIO_new(BIO_s_mem());
    bio = BIO_push(base64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, data, count);
    BIO_flush(bio);
    
    BUF_MEM * buffer;
    BIO_get_mem_ptr(bio, &buffer);
    char * result = malloc(buffer->length + 1);
    memcpy(result, buffer->data, buffer->length);
    result[buffer->length] = '\0';

    BIO_free_all(bio);
    return result;
}
