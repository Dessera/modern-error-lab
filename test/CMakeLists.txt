add_executable(test_flex_perf test_flex_perf.cpp)
target_compile_features(test_flex_perf PRIVATE cxx_std_23)
target_link_libraries(test_flex_perf PRIVATE ${FLEX_LIBNAME})

target_sources(test_flex_perf PRIVATE FILE_SET HEADERS FILES test_flex_perf.hpp)
