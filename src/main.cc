#include <iostream>
#include <string>
#include <cstdlib>

extern "C"
{
    extern char const _binary_hello_txt_start;
    extern char const _binary_hello_txt_end;
}

int main(int argc, char * argv[])
{
    std::size_t size = &_binary_hello_txt_end - &_binary_hello_txt_start;
    std::string hello = std::string(&_binary_hello_txt_start, size);

    std::cout << hello << std::endl;

    return EXIT_SUCCESS;
}