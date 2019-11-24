/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <cstdlib>

#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;
using openssl::BasicIO;
using openssl::CMS;
using openssl::Certificate;
using openssl::CertificateStore;
using openssl::CertificateStack;

using cli::App;
using cli::Argument;
using cli::Arguments;

namespace
{
    int verify(Arguments const & args)
    {
        auto const & filename = args.get('f');
        auto const & sign_filename = args.get('s');
        auto const & cert_filename = args.get('c');
        auto is_verbose = args.contains('v');

        int result = EXIT_FAILURE;
        try
        {
            auto file = BasicIO::openInputFile(filename);
            auto cms = CMS::fromFile(sign_filename);
            auto cert = Certificate::fromPEM(cert_filename);
            
            CertificateStore store;
            store.add(cert);

            CertificateStack certs;
            certs.push(cert);

            bool is_valid = cms.verify(certs, store, file, nullptr, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY, is_verbose);

            if (is_valid)
            {
                std::cout << "OK" << std::endl;
                result = EXIT_SUCCESS;
            }
            else
            {
                std::cout << "INVALID" << std::endl;
            }
        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }

        return result;
    }
}

int main(int argc, char* argv[])
{
    OpenSSL::init();

    App app("cms_verify", verify);
    app
        .setCopyright("Copyright (c) 2019 Falk Werner")
        .setDescription("Verifies file signatures produced with cms_sign")
        .setAdditionalInfo(
            "OpenSSL: create self-signed certificate and key\n"
            "\topenssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes\n"
            "\n"
            "Create signature:\n"
            "\tcms_sign -f any.file -p key.pem -c cert.pem > any.sig\n"
            "\n"
            "Examples:\n"
            "\tcms_verify -f any.file -s any.sig -c cert.pem\n"
        )
        .add(Argument('f', "file").setHelpText("File that was signed."))
        .add(Argument('s', "signature").setHelpText("File containing signature."))
        .add(Argument('c', "certificate").setHelpText("Certifiacte to sign (format: PEM)."))
        .add(Argument('v', "verbose").setHelpText("Print additional information to stderr.").setFlag().setOptional())
    ;

    return app.run(argc, argv);
}




