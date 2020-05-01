cmake_minimum_required (VERSION 3.10)
project(walk-directory VERSION 1.0.0 DESCRIPTION "walk directory example")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(C_WARNINGS -Wall -Wextra)

add_executable(walk-directory
	src/main.c
  src/walk_directory.c
  src/readdir.c
  src/ftw.c
)
