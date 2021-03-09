# libarchive-example

Extract a single file from an archive using libarchive

## Compile

    mkdir build
    cd build
    cmake ..
    make

## Create archive and run

    echo 42 > answer
    tar -cf test.tar answer
    ./archive-example -a test.tar answer out.txt
    cat out.txt
