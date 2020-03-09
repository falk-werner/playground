# How to embed resource files in C applications

This example how to use ld to embed a resource in C applications.

## Foundation

    ld -r -b binary -o resource.o hello.txt

The command above create an object file containing the following symbols:

    > nm resource.o 
    000000000000000d D _binary_hello_txt_end
    000000000000000d A _binary_hello_txt_size
    0000000000000000 D _binary_hello_txt_start

## Build and run

    mkdir build
    cd build
    cmake ..
    make
    ./print-resource

