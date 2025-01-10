# Save the current state
set(_original_WARN_DEPRECATED ${CMAKE_WARN_DEPRECATED})
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "Suppress deprecation warnings for external projects" FORCE)

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

# Disable building tests for tinyxml2
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "Do not build tests for tinyxml2")

# Add tinyxml2
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(tinyxml2)

# Disable building tests for tinyxml2
set(PUGIXML_BUILD_TESTS OFF CACHE BOOL "Do not build tests for pugixml")

# Fetch pugixml
FetchContent_Declare(
    pugixml
    URL https://github.com/zeux/pugixml/releases/download/v1.14/pugixml-1.14.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pugixml)

# Restore the original state
set(CMAKE_WARN_DEPRECATED ${_original_WARN_DEPRECATED} CACHE BOOL "Restore original deprecation warnings setting" FORCE)

# Enable testing
enable_testing()

# Add the test executable
add_executable(musxdomtests
	musxdomtests.cpp
	document.cpp
    entries.cpp
	header.cpp
	pool.cpp
    # options
    options/accidental_options.cpp
    options/alternate_notation_options.cpp
    options/augementation_dot_options.cpp
    options/barline_options.cpp
    options/beam_options.cpp
    options/clef_options.cpp
    options/flag_options.cpp
    options/grace_note_options.cpp
    options/key_signature_options.cpp
    options/line_curve_options.cpp
    options/misc_options.cpp
    options/multimeasure_rest_options.cpp
    options/music_spacing_options.cpp
    options/page_options.cpp
    options/piano_brace_bracket_options.cpp
    options/repeat_options.cpp
    options/smart_shape_options.cpp
    options/staff_options.cpp
    options/stem_options.cpp
    options/tie_options.cpp
    options/time_signature_options.cpp
    options/tuplet_options.cpp
    # others
	others/enclosure.cpp
	others/expression.cpp
	others/fonts.cpp
    others/frame.cpp
	others/layer_attributes.cpp
	others/instrument_used.cpp
    others/measure_number_region.cpp
    others/part_definition.cpp
	others/part_globals.cpp
    others/staff.cpp
    others/text_block.cpp
    others/text.cpp
    # details
	details/gfhold.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    # Extra warnings for Clang/AppleClang
    target_compile_options(musxdomtests PRIVATE -Wall -Wextra -Werror -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Extra warnings for MSVC
    target_compile_options(musxdomtests PRIVATE /Zc:__cplusplus /W4 /WX)
endif()

add_dependencies(musxdomtests musx)
target_sources(musxdomtests PUBLIC "${MUSX_ROOT_DIR}/src/musx/musx.h")

# Add the include paths
target_include_directories(musxdomtests PRIVATE "${MUSX_ROOT_DIR}/tests")
target_include_directories(musxdomtests PRIVATE "${MUSX_ROOT_DIR}/third_party/rapidxml")

# Link Google Test and the library to the test executable
target_link_libraries(musxdomtests PRIVATE
    gtest_main
	tinyxml2
    pugixml
    musx
)

# Add Precompiled Headers (PCH) for the test suite
target_precompile_headers(musxdomtests PRIVATE
    "${gtest_SOURCE_DIR}/include/gtest/gtest.h"
    "${MUSX_ROOT_DIR}/src/musx/musx.h"
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(musxdomtests
    DISCOVERY_TIMEOUT 60 # Set timeout to 60 seconds
    PROPERTIES
        ENVIRONMENT "GTEST_COLOR=1"
    EXTRA_ARGS --gtest_color=yes
)
