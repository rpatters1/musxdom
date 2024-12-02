# Enable testing
enable_testing()

# Add Google Test
include(FetchContent)
FetchContent_Declare(
	googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add tinyxml2
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(tinyxml2)

# Add the test executable
add_executable(musxdomtests
	musxdomtests.cpp
	fonts_tests.cpp
	header_tests.cpp
)

# Compile definitions
target_compile_definitions(musxdomtests PRIVATE MUSX_USE_TINYXML2)
target_compile_definitions(musxdomtests PRIVATE MUSX_USE_RAPIDXML)

# Add the include path for rapidxml
target_include_directories(musxdomtests PRIVATE "${CMAKE_SOURCE_DIR}/third_party/rapidxml")

# Link Google Test and the library to the test executable
target_link_libraries(musxdomtests
    gtest_main
	tinyxml2
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(musxdomtests)
