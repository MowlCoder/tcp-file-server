cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project("tcp-file-server")

include_directories(src src/commands src/network)

file(GLOB_RECURSE SRC_FILES src/server.cpp src/commands/*.cpp src/network/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()

project("tcp-file-client")

include_directories(src src/commands src/network)

file(GLOB_RECURSE SRC_FILES src/client.cpp src/commands/*.cpp src/network/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()