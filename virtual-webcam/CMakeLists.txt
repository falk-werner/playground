cmake_minimum_required (VERSION 3.10)
project(virtual-webcam VERSION 1.0.0 DESCRIPTION "Virtual WebCAM")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPNG REQUIRED libpng)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_executable(video-info
    src/video_info.c)

add_executable(put-image
    src/put_image.c
    src/png_image.c
)

target_compile_options(put-image PUBLIC
    ${LIBPNG_CFLAGS_OTHER}
)

target_link_libraries(put-image PUBLIC
    ${LIBPNG_LIBRARIES}
)

