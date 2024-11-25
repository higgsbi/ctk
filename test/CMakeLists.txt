cmake_minimum_required(VERSION 3.0)
project(test)

option(TEST_NAME "Test file to run" "strings.c")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
include_directories(${PROJECT_SOURCE_DIR}/../out/include/)
link_directories(${PROJECT_SOURCE_DIR}/../out/lib/)

add_executable(${PROJECT_NAME} ${TEST_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused-function -Wno-pointer-sign)
target_link_libraries(${PROJECT_NAME} ctk)
