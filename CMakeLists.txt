cmake_minimum_required(VERSION 3.28)
project(deema)

set(CMAKE_CXX_STANDARD 17)

add_executable(deema main.cpp args.h)

find_package(nlohmann_json REQUIRED)
find_package(httplib REQUIRED)

target_link_libraries(deema nlohmann_json::nlohmann_json)
target_link_libraries(deema httplib::httplib)