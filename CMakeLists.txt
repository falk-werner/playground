cmake_minimum_required (VERSION 3.1)
project(openssl-examples VERSION 1.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(openssl++
    lib/exception.cc
    lib/init.cc
    lib/bio.cc
    lib/private_key.cc
    lib/certificate.cc
    lib/cms.cc
    lib/base64.cc
)
target_include_directories(openssl++ PRIVATE lib include)
target_compile_options(openssl++ PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(openssl++ PUBLIC ${OPENSSL_LIBRARIES})


add_executable(cms_sign
    src/cms_sign.cc)
target_include_directories(cms_sign PRIVATE src include)
target_link_libraries(cms_sign openssl++)

add_executable(cms_verify
    src/cms_verify.cc)
target_include_directories(cms_verify PRIVATE src include)
target_link_libraries(cms_verify openssl++)
