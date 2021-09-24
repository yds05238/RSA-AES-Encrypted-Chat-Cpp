cmake_minimum_required(VERSION 2.8)

project(AES)

set(SOURCES
	src/AES.cpp
	include/AES.h)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (UNIX)
	target_link_libraries(AES pthread)
endif ()