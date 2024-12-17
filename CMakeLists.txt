cmake_minimum_required(VERSION 3.10)
project(musx)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/bigobj)
endif()

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Enable testing globally
enable_testing()

set(BUILD_TESTING OFF CACHE BOOL "Disable tests for external dependencies" FORCE)

# Add the tests directory
add_subdirectory(tests)
