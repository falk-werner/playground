cmake_minimum_required (VERSION 3.10)
project(dbus-example VERSION 1.0.0 DESCRIPTION "D-Bus example using glib2.0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-unix-2.0)

set(CMAKE__STANDARD 99)

set(C_WARNINGS -Wall -Wextra)

add_executable(greeter_service
    src/service/main.c
    ${CMAKE_BINARY_DIR}/gen/greeter.c
)

target_include_directories(greeter_service PUBLIC ${CMAKE_BINARY_DIR}/gen ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_compile_options(greeter_service PUBLIC ${GLIB_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER})
target_link_libraries(greeter_service PUBLIC ${GLIB_LIBRARIES} ${GIO_LIBRARIES})

add_executable(greet
    src/client/main.c
    ${CMAKE_BINARY_DIR}/gen/greeter.c
)

target_include_directories(greet PUBLIC ${CMAKE_BINARY_DIR}/gen ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_compile_options(greet PUBLIC ${GLIB_CFLAGS_OTHER} ${GIO_CFLAGS_OTHER})
target_link_libraries(greet PUBLIC ${GLIB_LIBRARIES} ${GIO_LIBRARIES})


add_custom_command(OUTPUT  ${CMAKE_BINARY_DIR}/gen/greeter.h  ${CMAKE_BINARY_DIR}/gen/greeter.c
    COMMAND mkdir -p  ${CMAKE_BINARY_DIR}/gen
    COMMAND cd ${CMAKE_BINARY_DIR}/gen && gdbus-codegen --generate-c-code greeter ${CMAKE_SOURCE_DIR}/greeter.xml
    DEPENDS ${CMAKE_SOURCE_DIR}/greeter.xml
)

