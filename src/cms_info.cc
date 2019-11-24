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