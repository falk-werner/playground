cmake_minimum_required (VERSION 3.1)
project(openssl-examples VERSION 1.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cms_sign
    src/cms_sign.c)
target_include_directories(cms_sign PRIVATE src)
target_compile_options(cms_sign PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(cms_sign PUBLIC ${OPENSSL_LIBRARIES})

