cmake_minimum_required (VERSION 3.10)
project(curl-download VERSION 1.0.0 DESCRIPTION "Download a file using cURL")

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)
pkg_check_modules(GTEST REQUIRED gtest_main)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_WARNINGS -Wall -Wextra)

add_library(fetch STATIC
    src/fetch.c)

target_include_directories(fetch PUBLIC
    include
    src
    ${LIBCURL_INCLUDE_DIRS}
)

target_compile_options(fetch PUBLIC
    ${LIBCURL_CFLAGS}
)

target_link_libraries(fetch PUBLIC
    ${LIBCURL_LIBRARIES}
)

add_executable(download
    src/main.c
)

target_link_libraries(download PUBLIC
    fetch
)

add_executable(alltests
    test-src/net/socket.cpp
    test-src/net/socketpair.cpp
    test-src/net/http_server.cpp
    test-src/test_fetch.cpp
)

target_include_directories(alltests PRIVATE
    include
    src
    test-src
)

target_compile_options(alltests PRIVATE
    ${GTEST_CFLAGS}
)

target_link_libraries(alltests
    fetch
    ${CMAKE_THREAD_LIBS_INIT}
    ${GTEST_LIBRARIES}
)
