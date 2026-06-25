add_executable(benchmarks
    bench_lookup.cpp
)

target_include_directories(benchmarks PRIVATE
    "${MUSX_ROOT_DIR}/src"
    "${MUSX_ROOT_DIR}/third_party/rapidxml"
)

target_link_libraries(benchmarks PRIVATE
    musx
    tinyxml2
    pugixml
)

target_compile_features(benchmarks PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(benchmarks PRIVATE /bigobj /W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    target_compile_options(benchmarks PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
