cmake_minimum_required(VERSION 3.18)

project("AoC2018")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#add_subdirectory(alg_dir ${CMAKE_CURRENT_BINARY_DIR}/algs)

add_executable(aoc ${SOURCE_FILES})
#target_link_libraries(aoc crypto)
target_link_libraries(aoc ortools)
target_include_directories(aoc SYSTEM PRIVATE 
    #/usr/include/openssl
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/alg_dir/structures/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/alg_dir/graph/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/alg_dir/util/include
    PRIVATE /usr/include/ortools
    )
