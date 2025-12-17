add_executable(test_flex_perf test_flex_perf.cpp)
target_compile_features(test_flex_perf PRIVATE cxx_std_23)
target_link_libraries(test_flex_perf PRIVATE ${FLEX_LIBNAME})

target_sources(
    test_flex_perf
    PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
           ${CMAKE_CURRENT_SOURCE_DIR}/test_flex_perf.hpp)
