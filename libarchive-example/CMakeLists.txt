cmake_minimum_required(VERSION 3.10)
project(libarchive-example)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

include_directories(
    ${LIBARCHIVE_INCLUDE_DIRS}
)

add_compile_options(
    ${LIBARCHIVE_CFLAGS_OTHER}
)

link_libraries(
    ${LIBARCHIVE_LIBRARIES}
)

add_executable(archive_example
    src/main.c
)
