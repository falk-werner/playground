cmake_minimum_required (VERSION 3.10)
project(virtual-webcam VERSION 1.0.0 DESCRIPTION "Virtual WebCAM")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPNG REQUIRED libpng)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_library(util STATIC
    src/util/yuv.c
    src/util/png_image.c
    src/util/put_image.c
)

target_compile_options(util PUBLIC
    ${LIBPNG_CFLAGS_OTHER}
)

target_link_libraries(util PUBLIC
    ${LIBPNG_LIBRARIES}
)

target_include_directories(util PUBLIC
    src/util
)

add_executable(video-info
    src/video_info/main.c)

add_executable(put-image
    src/put_image/main.c
)

target_link_libraries(put-image PUBLIC
    util
)

add_executable(vmenu
    src/vmenu/item_list.c
    src/vmenu/main.c
)

target_link_libraries(vmenu PUBLIC
    util
    menu
    ncurses
)
