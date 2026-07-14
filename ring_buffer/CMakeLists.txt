cmake_minimum_required(VERSION 3.16)

project(RingBuffer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ring_buffer
    src/main.cpp
    src/RingBuffer.cpp
    src/Logger.cpp
    src/MemoryPool.cpp
)

target_include_directories(ring_buffer
    PRIVATE
        include
)

target_compile_options(ring_buffer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)