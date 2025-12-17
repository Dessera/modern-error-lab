cmake_minimum_required(VERSION 3.28.2)
project(
    modern-error-lab
    VERSION 0.1.0
    DESCRIPTION "Error handling laboratory for modern cpp."
    LANGUAGES CXX)

set(PNAME modern-error-lab)

set(FLEX_PNAME flex)
set(FLEX_LIBNAME ${FLEX_PNAME})

set(TOKEN_PNAME token)
set(TOKEN_LIBNAME ${TOKEN_PNAME}-lib)
set(TOKEN_EXENAME ${TOKEN_PNAME}-exe)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(${FLEX_LIBNAME} INTERFACE)
target_compile_features(${FLEX_LIBNAME} INTERFACE cxx_std_23)

target_include_directories(
    ${FLEX_LIBNAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

add_library(${TOKEN_LIBNAME} SHARED)
set_target_properties(${TOKEN_LIBNAME} PROPERTIES OUTPUT_NAME ${TOKEN_PNAME}
                                                  CXX_VISIBILITY_PRESET hidden)
target_compile_features(${TOKEN_LIBNAME} PRIVATE cxx_std_23)
target_link_libraries(${TOKEN_LIBNAME} PUBLIC ${FLEX_LIBNAME})

generate_export_header(${TOKEN_LIBNAME} EXPORT_FILE_NAME
                       ${TOKEN_PNAME}_export.h)
target_sources(
    ${TOKEN_LIBNAME}
    PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
           ${CMAKE_CURRENT_BINARY_DIR}/${TOKEN_PNAME}_export.h)

target_include_directories(
    ${TOKEN_LIBNAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

add_executable(${TOKEN_EXENAME})
set_target_properties(${TOKEN_EXENAME} PROPERTIES OUTPUT_NAME ${TOKEN_PNAME})
target_compile_features(${TOKEN_EXENAME} PRIVATE cxx_std_23)
target_link_libraries(${TOKEN_EXENAME} PRIVATE ${TOKEN_LIBNAME})

add_subdirectory(src)
add_subdirectory(test)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PNAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PNAME}-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PNAME})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PNAME}-config-version.cmake
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion)

install(
    TARGETS ${FLEX_LIBNAME} ${TOKEN_LIBNAME}
    EXPORT ${PNAME}-targets
    FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT ${PNAME}-targets
    NAMESPACE ${PNAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PNAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PNAME}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PNAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PNAME})

install(DIRECTORY include/${FLEX_PNAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/${TOKEN_PNAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
