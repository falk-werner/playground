cmake_minimum_required (VERSION 3.10)
project(virtual-webcam VERSION 1.0.0 DESCRIPTION "Virtual WebCAM")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPNG REQUIRED libpng)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_library(vstream STATIC
    src/vstream/yuv.c
    src/vstream/png_image.c
    src/vstream/video_stream.c
)

target_compile_options(vstream PUBLIC
    ${LIBPNG_CFLAGS_OTHER}
)

target_link_libraries(vstream PUBLIC
    ${LIBPNG_LIBRARIES}
)

target_include_directories(vstream PUBLIC
    src/vstream
)

add_executable(video-info
    src/video_info/main.c)

add_executable(put-image
    src/put_image/main.c
)

target_link_libraries(put-image PUBLIC
    vstream
)

add_executable(vmenu
    src/vmenu/item_list.c
    src/vmenu/main.c
)

target_link_libraries(vmenu PUBLIC
    vstream
    menu
    ncurses
)
