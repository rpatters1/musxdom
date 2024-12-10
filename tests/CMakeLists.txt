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
	document_tests.cpp
	header_tests.cpp
	pool_tests.cpp
    # options
    options/barline_options_tests.cpp
    options/clef_options_tests.cpp
    options/line_curve_options_tests.cpp
    options/page_options_tests.cpp
    options/repeat_options_tests.cpp
    # others
	others/enclosure_tests.cpp
	others/expression_tests.cpp
	others/fonts_tests.cpp
    others/text_tests.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    # Extra warnings for Clang/AppleClang
    target_compile_options(musxdomtests PRIVATE -Wall -Wextra -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Extra warnings for MSVC
    target_compile_options(musxdomtests PRIVATE /Zc:__cplusplus /W4 /WX)
endif()

# Compile definitions
target_compile_definitions(musxdomtests PRIVATE MUSX_USE_TINYXML2)
target_compile_definitions(musxdomtests PRIVATE MUSX_USE_RAPIDXML)

# Add the include paths
target_include_directories(musxdomtests PRIVATE "${CMAKE_SOURCE_DIR}/tests")
target_include_directories(musxdomtests PRIVATE "${CMAKE_SOURCE_DIR}/third_party/rapidxml")

# Link Google Test and the library to the test executable
target_link_libraries(musxdomtests
    gtest_main
	tinyxml2
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(musxdomtests
    PROPERTIES
        ENVIRONMENT "GTEST_COLOR=1"
    EXTRA_ARGS --gtest_color=yes
)
