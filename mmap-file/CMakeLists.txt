cmake_minimum_required (VERSION 3.10)
project(mmap VERSION 1.0.0 DESCRIPTION "mmap example")

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(C_WARNINGS -Wall -Wextra)

add_executable(checksum
    src/main.c
    src/md5.c
    src/strategy.c
    src/mmap.c
    src/readall.c
    src/buffered.c
)

target_compile_options(checksum PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(checksum PUBLIC ${OPENSSL_LIBRARIES})

