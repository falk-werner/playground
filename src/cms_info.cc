/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>
#include <cstdlib>

#include "openssl++/openssl++.hpp"
#include "cli/cli.hpp"

using openssl::OpenSSL;
using openssl::BasicIO;
using openssl::Certificate;
using openssl::CMS;

using cli::App;
using cli::Argument;
using cli::Arguments;

namespace
{
    int info(Arguments const & args)
    {
        auto const & filename = args.get('s');
        int result = EXIT_FAILURE;

        try
        {
            CMS cms = CMS::fromFile(filename);
            cms.dump();
            result = EXIT_SUCCESS;
        }
        catch (std::exception const & ex)
        {
            std::cerr << ex.what() << std::endl;;
        }

        return result;
    }
}

int main(int argc, char * argv[])
{
    OpenSSL::init();

    App app("cms_info", info);
    app
        .setCopyright("Copyright (c) 2019 Falk Werner")
        .setDescription("Prints signature")
        .add(Argument('s', "signature").setHelpText("File containing signature."))
    ;

    return app.run(argc, argv);
}