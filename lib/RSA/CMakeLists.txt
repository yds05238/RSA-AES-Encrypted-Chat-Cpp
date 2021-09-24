cmake_minimum_required(VERSION 2.8)

project(RSA)

set(SOURCES
    src/cleanup_keys.sh
    src/generate_keys.sh 
	src/bignum.cpp
    src/normal_bignum.cpp
    src/RSA.cpp
    include/bignum.h
    include/RSA.h)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (UNIX)
	target_link_libraries(RSA pthread)
endif ()