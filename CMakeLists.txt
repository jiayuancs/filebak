cmake_minimum_required(VERSION 3.0)

project(FILEBAK)

add_compile_options(-Wall -std=c++17)

set(CMAKE_BUILD_TYPE Debug)

include_directories(./include)

# add_executable(main test/test_packer.cpp src/packer.cpp)
# add_executable(main test/test_filebase.cpp src/filebase.cpp)
# add_executable(main test/test_link.cpp)
add_executable(main test/test_packer.cpp src/packer.cpp src/filebase.cpp)