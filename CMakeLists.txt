cmake_minimum_required(VERSION 3.5)


project(
    sm
    VERSION 1.0
    LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

enable_testing()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
include(CodeCoverage)
append_coverage_compiler_flags()
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

                              
add_subdirectory(lib)
add_subdirectory(smi)

