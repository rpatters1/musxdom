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

add_executable(text_insertion_benchmark EXCLUDE_FROM_ALL
    bench_text_insertion.cpp
)

target_include_directories(text_insertion_benchmark PRIVATE
    "${MUSX_ROOT_DIR}/src"
)

target_link_libraries(text_insertion_benchmark PRIVATE
    musx
)

target_compile_features(text_insertion_benchmark PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(benchmarks PRIVATE /bigobj /W4 /WX)
    target_compile_options(text_insertion_benchmark PRIVATE /bigobj /W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    target_compile_options(benchmarks PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(text_insertion_benchmark PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
