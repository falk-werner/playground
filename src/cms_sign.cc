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

#include <iostream>
#include <cstdlib>

#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;
using openssl::PrivateKey;
using openssl::BasicIO;
using openssl::Certificate;
using openssl::CMS;

using cli::App;
using cli::Argument;
using cli::Arguments;

static int sign(Arguments const & args)
{
    auto const & filename = args.get('f');
    auto const & key_file = args.get('p');
    auto const & cert_file = args.get('c');
    bool is_verbose = args.contains('v');

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

int main(int argc, char * argv[])
{
    OpenSSL::init();

    App app("cms_sign", sign);
    app
        .setCopyright("Copyright (c) 2019 Falk Werner")
        .setDescription("Prints signature of a file using OpenSSL Cryptographic Message Syntax (CMS).")
        .setAdditionalInfo(
            "OpenSSL: create self-signed certificate and key\n"
            "\topenssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes\n"
            "\n"
            "Examples:\n"
            "\tcms_sign -f any.file -p key.pem -c cert.pem\n"
        )
        .add(Argument('f', "file").setHelpText("File to sign."))
        .add(Argument('p', "private-key").setHelpText("Private key to sign (format: PEM)."))
        .add(Argument('c', "certificate").setHelpText("Certifiacte to sign (format: PEM)."))
        .add(Argument('v', "verbose").setHelpText("Print additional information to stderr.").setFlag().setOptional())
    ;

    return app.run(argc, argv);
}
