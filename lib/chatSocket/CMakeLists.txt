cmake_minimum_required(VERSION 2.8)

project(chatSocket)

set(SOURCES
	src/chatNode.cpp
	src/chatServer.cpp
	src/chatClient.cpp
	include/chatNode.h
	include/chatClient.h
	include/chatServer.h)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (UNIX)
	target_link_libraries(chatSocket pthread)
endif ()