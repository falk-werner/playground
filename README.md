[![Build Status](https://travis-ci.org/falk-werner/playground.svg?branch=master)](https://travis-ci.org/falk-werner/playground)

# Playground

Contains non-productive examples.

## Examples

*   **[embed-resources](embed-resources):** How to embed resources in C applications
*   **[png-example](png-example):** How to read and write PNG files using libpng
*   **[popen-example](popen-example):** How to execute a command using popen
*   **[openssl-examples](openssl-examples):** How to sign files using OpenSSL

## Build

Use CMake to build an example:

    mkdir <example-dir>/build
    cd <examle-dir>/build
    cmake ..
    make

Note that some examples might need prerequisites to build, e.g. libssl-dev for openssl-examples.
