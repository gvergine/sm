find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(GLOB c_sources *.c)

include(gengetopt)

add_gengetopt_files(cmdline)

bison_target(parser sm.y ${CMAKE_CURRENT_BINARY_DIR}/sm.tab.c)
flex_target(scanner sm.l  ${CMAKE_CURRENT_BINARY_DIR}/sm_lexer.c)
add_flex_bison_dependency(scanner parser)

add_executable(smi ${GGO_C} ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS} ${c_sources})
target_include_directories(smi PUBLIC ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(smi sm-shared)
target_compile_options(smi PRIVATE -Werror -Wall -pedantic)

include(GNUInstallDirs)
install(TARGETS smi
    EXPORT smi-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
find_program (BASH_PROGRAM bash)
add_test (smi-tests ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.sh ${CMAKE_CURRENT_BINARY_DIR}/smi ${CMAKE_CURRENT_SOURCE_DIR}/tests)
setup_target_for_coverage_lcov(NAME smi_coverage
                              EXECUTABLE ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.sh ${CMAKE_CURRENT_BINARY_DIR}/smi  ${CMAKE_CURRENT_SOURCE_DIR}/tests
                              BASE_DIRECTORY "${PROJECT_BINARY_DIR}/smi/"
                              EXCLUDE "${PROJECT_SOURCE_DIR}/smi/main.c"
                              EXCLUDE "${PROJECT_BINARY_DIR}/smi/cmdline.c"
                              EXCLUDE "${PROJECT_BINARY_DIR}/smi/sm_lexer.c"
                              EXCLUDE "${PROJECT_BINARY_DIR}/smi/sm.tab.c"
                              EXCLUDE "${PROJECT_BINARY_DIR}/smi/sm.y"
                              EXCLUDE "${PROJECT_BINARY_DIR}/smi/sm.l"
                              EXCLUDE "${PROJECT_SOURCE_DIR}/smi/tests/*"                              
                                                            )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

