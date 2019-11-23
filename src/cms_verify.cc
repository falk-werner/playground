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

using cli::App;
using cli::Argument;
using cli::Arguments;

namespace
{
    int verify(Arguments const & args)
    {
        std::cerr << "error: not implemented yet" << std::endl;
        return EXIT_FAILURE;
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




