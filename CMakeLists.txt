cmake_minimum_required (VERSION 3.10)
project(png-example)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPNG REQUIRED libpng)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

include_directories(
    ${LIBPNG_INCLUDE_DIRS}
)

add_compile_options(
    ${LIBPNG_CFLAGS_OTHER}
)

link_libraries(
    ${LIBPNG_LIBRARIES}
)

add_executable(png_turn_gray
    src/png_image.c
    src/main.c
)