# walk-directory

This example shows, how to walk over files in a directory.

    mkdir build
    cd build
    cmake ..
    make
    ./walk-directory .

There are two different implementations included:

*    **ftw:** uses ftw to walk directory (see man 3 ftw)
*    **readdir:** a naive implementation based on readdir

In general `ftw` should be prefered, since it's standard,
has more features (e.g. following symlinks or cdw) and is aware of more
corner cases (e.g. max concurrent open file descriptors for readdir).

But there is one disadvantage of ftw: it's callback is stateless.  
If state should be used, it must be provided by global variables :-/
