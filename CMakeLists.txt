cmake_minimum_required(VERSION 3.5)


project(
    sm
    VERSION 1.0.0
    LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} -s 
    --trace-children=yes --leak-check=full --error-exitcode=255 --log-fd=2
    --show-leak-kinds=all --errors-for-leak-kinds=all")
include (CTest)
enable_testing()
include(CodeCoverage)
append_coverage_compiler_flags()
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")


add_subdirectory(lib)
add_subdirectory(smi)

