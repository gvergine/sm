cmake_minimum_required(VERSION 3.5)


project(
    sm
    VERSION 1.0
    LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
include (CTest)
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes --leak-check=full")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=255 --log-fd=2")
enable_testing()
include(CodeCoverage)
append_coverage_compiler_flags()
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")


add_subdirectory(lib)
add_subdirectory(smi)

