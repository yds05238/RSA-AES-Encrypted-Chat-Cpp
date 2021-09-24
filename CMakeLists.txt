cmake_minimum_required(VERSION 2.8)

if (CMAKE_VERSION VERSION_LESS "3.1")
  add_definitions(-std=c++17)
else()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

project(encryptedChat)

# AES library
add_subdirectory(lib/AES)
include_directories(lib/AES/include)

# create encryptedChatServer
add_executable(encryptedChatServer Chat/encryptedChatServer.cpp)
# create encryptedChatClient
add_executable(encryptedChatClient Chat/encryptedChatClient.cpp)

# link library
target_link_libraries(encryptedChatServer AES)
target_link_libraries(encryptedChatClient AES)

# install
install(TARGETS encryptedChatServer DESTINATION bin)
install(TARGETS encryptedChatClient DESTINATION bin)