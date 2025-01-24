# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.22)

project(abppr)

# Set the C++ standard version to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Add optimizations
SET(CMAKE_CXX_FLAGS "-O3 -march=native -flto=auto -w -Wl,-rpath -Wl,--enable-new-dtags")


# Include Boost libraries
find_package(Boost REQUIRED COMPONENTS filesystem system date_time program_options)


# Set project directories
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)


# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)


# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Include directories for each target
add_library(graph ${SOURCE_DIR}/graph.cpp ${INCLUDE_DIR}/graph.hpp)
target_include_directories(graph PUBLIC ${INCLUDE_DIR})


add_library(utils ${SOURCE_DIR}/utils.cpp ${INCLUDE_DIR}/utils.hpp)
target_include_directories(utils PUBLIC ${INCLUDE_DIR})


add_library(algo ${SOURCE_DIR}/algo.cpp ${INCLUDE_DIR}/algo.hpp)
target_include_directories(algo PUBLIC ${INCLUDE_DIR})


# Add executable
add_executable(abppr ${SOURCE_DIR}/abppr.cpp)
target_link_libraries(abppr PRIVATE graph algo utils pthread dl rt Boost::program_options)


# Move the runnable file from build dir to parent dir
add_custom_command(TARGET abppr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:abppr> ${PROJECT_SOURCE_DIR}/abppr
)
