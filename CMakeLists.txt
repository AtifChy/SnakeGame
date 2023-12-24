cmake_minimum_required(VERSION 3.20)
project(SnakeGame VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

find_package(raylib 4.0 REQUIRED)
find_package(nlohmann_json 3.9.1 REQUIRED)

add_executable(SnakeGame main.cpp)

target_link_libraries(SnakeGame PRIVATE raylib)
target_link_libraries(SnakeGame PRIVATE nlohmann_json::nlohmann_json)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
