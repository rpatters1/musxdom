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
