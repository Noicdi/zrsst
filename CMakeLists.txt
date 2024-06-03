cmake_minimum_required(VERSION 3.22)

project(zrsst VERSION 0.0.1)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src project_src)
add_executable(${PROJECT_NAME} ${project_src})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS -Werror)

add_subdirectory(tests)
