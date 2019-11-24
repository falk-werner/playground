/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
    bool write_to_file = args.contains('o');

    int result = EXIT_FAILURE;

    try
    {
        auto key = PrivateKey::fromPEM(key_file);
        auto cert = Certificate::fromPEM(cert_file);
        auto file = BasicIO::openInputFile(filename);

        auto cms = CMS::sign(cert, key, nullptr, file, CMS_DETACHED | CMS_NOCERTS | CMS_BINARY);

        auto out = (write_to_file) ? BasicIO::openOutputFile(args.get('o')) : BasicIO::getStdout();
        cms.saveToBIO(out);

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
        .add(Argument('o', "out-file").setHelpText("File to store signature to (if omitted, write signature to stdout).").setOptional())
        .add(Argument('v', "verbose").setHelpText("Print additional information to stderr.").setFlag().setOptional())
    ;

    return app.run(argc, argv);
}
