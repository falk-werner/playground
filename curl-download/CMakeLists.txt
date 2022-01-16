cmake_minimum_required (VERSION 3.10)
project(curl-download VERSION 1.0.0 DESCRIPTION "Download a file using cURL")

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)
pkg_check_modules(ATOMIC_OPS REQUIRED atomic_ops)
pkg_check_modules(GTEST REQUIRED gtest_main)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_WARNINGS -Wall -Wextra)

add_library(download STATIC
    src/download.c)

target_include_directories(download PUBLIC
    include
    src
    ${LIBCURL_INCLUDE_DIRS}
    ${ATOMIC_OPS_INCLUDE_DIRS}
)

target_compile_options(download PUBLIC
    ${LIBCURL_CFLAGS}
    ${ATOMIC_OPS_CFLAGS}
)

target_link_libraries(download PUBLIC
    ${LIBCURL_LIBRARIES}
    ${ATOMIC_OPS_LIBRARIES}
)

add_executable(fetch
    src/main.c
)

target_link_libraries(fetch PUBLIC
    download
)

add_executable(alltests
    test-src/curl_environment.cpp
    test-src/net/socket.cpp
    test-src/net/socketpair.cpp
    test-src/net/http_server.cpp
    test-src/test_download.cpp
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
    download
    ${CMAKE_THREAD_LIBS_INIT}
    ${GTEST_LIBRARIES}
)
