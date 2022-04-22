cmake_minimum_required (VERSION 3.10)
project(password-utils)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(dump_passwd
    src/dump_passwd.c
)

add_executable(is_password_expired
    src/is_password_expired.c
)

add_executable(create_password_hash
    src/create_password_hash.c
)

target_link_libraries(create_password_hash PRIVATE crypt)
