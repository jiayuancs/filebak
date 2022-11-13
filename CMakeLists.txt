cmake_minimum_required(VERSION 3.16)
project(FILEBAK)

# 可执行文件名
set(EXE_FILE_NAME filebak)

# 源文件
aux_source_directory(./src SRC_FILES)
include_directories(./include)

# openssl
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# 编译选项
add_compile_options(-w -O2 -std=c++17 -lcrypto)
set(CMAKE_BUILD_TYPE Release)

add_executable(${EXE_FILE_NAME} ${SRC_FILES})

target_link_libraries(${EXE_FILE_NAME} OpenSSL::Crypto)

install(TARGETS ${EXE_FILE_NAME} DESTINATION bin)