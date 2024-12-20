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

set(MUSX_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/musx/factory/FieldPopulatorsOptions.cpp
    ${CMAKE_SOURCE_DIR}/src/musx/factory/FieldPopulatorsOthers.cpp
    # Add more files here as needed
)

# Create a static library for the project
add_library(musx STATIC ${MUSX_SRC_FILES})

target_compile_definitions(musx PRIVATE MUSX_THROW_ON_UNKNOWN_XML)

# Ensure the library uses the correct include paths
target_include_directories(musx PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Add the tests directory
add_subdirectory(tests)
