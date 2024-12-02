cmake_minimum_required(VERSION 3.10)
project(musx)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add the tests directory
add_subdirectory(tests)
