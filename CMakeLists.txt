cmake_minimum_required(VERSION 3.16)
project(FILEBAK)

# 可执行文件名
set(EXE_FILE_NAME filebak)
# main函数所在文件
# set(MAIN_FILE_PATH test/test_task.cpp)
# 源文件
aux_source_directory(./src SRC_FILES)
include_directories(./include)

# openssl
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# 编译选项
add_compile_options(-Wall -std=c++17 -lcrypto)
set(CMAKE_BUILD_TYPE Debug)
add_executable(${EXE_FILE_NAME} ${MAIN_FILE_PATH} ${SRC_FILES})

target_link_libraries(${EXE_FILE_NAME} OpenSSL::Crypto)