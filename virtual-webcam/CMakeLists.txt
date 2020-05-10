cmake_minimum_required (VERSION 3.10)
project(virtual-webcam VERSION 1.0.0 DESCRIPTION "Virtual WebCAM")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPNG REQUIRED libpng)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_executable(test
    src/main.c
    src/png_image.c
)

target_compile_options(test PUBLIC
    ${LIBPNG_CFLAGS_OTHER}
)

target_link_libraries(test PUBLIC
    ${LIBPNG_LIBRARIES}
)
