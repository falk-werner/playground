# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 3.1)
project(openssl-examples VERSION 1.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(openssl++ STATIC
    lib/openssl++/exception.cc
    lib/openssl++/init.cc
    lib/openssl++/basic_io.cc
    lib/openssl++/private_key.cc
    lib/openssl++/certificate.cc
    lib/openssl++/certificate_store.cc
    lib/openssl++/certificate_stack.cc
    lib/openssl++/cms.cc
)
target_include_directories(openssl++ PRIVATE lib include)
target_compile_options(openssl++ PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(openssl++ PUBLIC ${OPENSSL_LIBRARIES})

add_library(cli STATIC
    lib/cli/app.cc
    lib/cli/argument.cc
    lib/cli/arguments.cc
    lib/cli/options.cc
)
target_include_directories(cli PRIVATE lib include)

add_executable(cms_sign
    src/cms_sign.cc)
target_include_directories(cms_sign PRIVATE src include)
target_link_libraries(cms_sign openssl++ cli)

add_executable(cms_info
    src/cms_info.cc)
target_include_directories(cms_info PRIVATE src include)
target_link_libraries(cms_info openssl++ cli)

add_executable(cms_verify
    src/cms_verify.cc)
target_include_directories(cms_verify PRIVATE src include)
target_link_libraries(cms_verify openssl++ cli)
