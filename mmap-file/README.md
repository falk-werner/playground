# mmap-file

Example of mmap to compute MD5 checksum of files.

## Build and run

    mkdir build
    cd build
    cmake ..
    make
    ./checksum -h

## Comparison

Create a large file (1 GByte) with random contents:

    dd if=/dev/urandom of=sample.file bs=1G count=1 iflag=fullblock

Use `time` measure using different strategies:
*(repeat a few times to avoid caching effects)*

**mmap**

    time ./checksum sample.file 
    0c94cbf600b6cf6eb7bfa0b855dc2b59 sample.file

    real    0m1,774s
    user    0m1,610s
    sys     0m0,128s

**buffered**

    time ./checksum -s buffered sample.file 
    0c94cbf600b6cf6eb7bfa0b855dc2b59 sample.file
    
    real    0m2,043s
    user    0m1,725s
    sys     0m0,285s

**readall**

    time ./checksum -s readall sample.file
    0c94cbf600b6cf6eb7bfa0b855dc2b59 sample.file
    
    real    0m2,489s
    user    0m1,656s
    sys     0m0,806s