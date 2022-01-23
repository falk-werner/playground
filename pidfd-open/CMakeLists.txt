cmake_minimum_required (VERSION 3.10)
project(pidfd-open VERSION 1.0.0 DESCRIPTION "Demo of pidfd_open")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(C_WARNINGS -Wall -Wextra)

add_executable(pidfd_open_demo
    src/main.c
    src/pidfd_open.c)

target_include_directories(pidfd_open_demo PRIVATE
    include
    src)

