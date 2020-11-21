find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(GLOB c_sources *.c)
list(REMOVE_ITEM c_sources main.c)

include(gengetopt)

add_gengetopt_files(cmdline)

bison_target(parser sm.y ${CMAKE_CURRENT_BINARY_DIR}/sm.tab.c)
flex_target(scanner sm.l  ${CMAKE_CURRENT_BINARY_DIR}/sm_lexer.c)
add_flex_bison_dependency(scanner parser)

add_library(smi-lib STATIC ${c_sources} ${GGO_C} ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS})
target_include_directories(smi-lib PUBLIC ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(smi-lib PRIVATE -Werror -Wall -pedantic)

add_executable(smi main.c)
target_link_libraries(smi smi-lib sm-shared)
target_compile_options(smi PRIVATE -Werror -Wall -pedantic)

include(GNUInstallDirs)
install(TARGETS smi
    EXPORT smi-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
file(GLOB test_sources tests/*.c)
find_package(Threads REQUIRED)
find_package(Check REQUIRED)
add_executable(smi-unit-tests ${test_sources})
target_link_libraries(smi-unit-tests PRIVATE sm-shared smi-lib ${CHECK_LIBRARIES} subunit Threads::Threads rt m)
add_test(smi-unit-tests smi-unit-tests)
setup_target_for_coverage_lcov(NAME smi_coverage
                              EXECUTABLE smi-unit-tests
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

