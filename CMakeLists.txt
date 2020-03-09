cmake_minimum_required (VERSION 3.10)
project(print-resource VERSION 1.0.0 DESCRIPTION "Webfuse daemon")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(C_WARNINGS -Wall -Wextra)

add_custom_target(resource
    COMMAND ld -r -b binary -o ${CMAKE_CURRENT_BINARY_DIR}/resource.o hello.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resource
)


add_executable(print-resource
	src/main.cc
    ${CMAKE_CURRENT_BINARY_DIR}/resource.o
)
add_dependencies(print-resource resource)

SET_SOURCE_FILES_PROPERTIES(
  ${CMAKE_CURRENT_BINARY_DIR}/resource.o
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
