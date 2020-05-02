# find-in-files

Combines path traversal and mmap to implement a simple find-in-files.

**Note:** `egrep <pattern> -r <path>` is still faster and has more features.

## Compile and run

    mkdir build
    cd build
    cmake ..
    make
    ./find-in-files munmap
