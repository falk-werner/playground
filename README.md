[![Build Status](https://travis-ci.org/falk-werner/playground.svg?branch=master)](https://travis-ci.org/falk-werner/playground)

# Playground

Contains non-productive examples.

## Examples

*   **[embed-resources](embed-resources):** How to embed resources in C applications
*   **[png-example](png-example):** How to read and write PNG files using libpng
*   **[popen-example](popen-example):** How to execute a command using popen
*   **[openssl-examples](openssl-examples):** How to sign files using OpenSSL
*   **[walk-directory](walk-directory):** Traverse a directory structure
*   **[mmap-file](mmap-file):** Compute MD5 checksum using mmap
*   **[find-in-files](find-in-files):** Prints file names containig a given text
*   **[dbus-example](dbus-example):** Inter-Process communication using D-BUS (via glib)
*   **[virtual-webcam](virtual-webcam):** Virtual WebCAM using v4l2loopback device

## Build

Use CMake to build an example:

    mkdir <example-dir>/build
    cd <examle-dir>/build
    cmake ..
    make

Note that some examples might need prerequisites to build, e.g. libssl-dev for openssl-examples.

## Using Docker

The provided Dockerfile can be user to create an image containing all examples:

    docker build --rm --build-arg "USER_ID=`id -u`" --tag playground .

You can run the image using the following command:

    docker run --rm --user="`id -u`" -it playground
