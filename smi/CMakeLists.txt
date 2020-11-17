find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(GLOB c_sources *.c)

include(gengetopt)

add_gengetopt_files(cmdline)


bison_target(parser sm.y ${CMAKE_CURRENT_BINARY_DIR}/sm.tab.c)
flex_target(scanner sm.l  ${CMAKE_CURRENT_BINARY_DIR}/sm_lexer.c)
add_flex_bison_dependency(scanner parser)


add_executable(smi ${c_sources} ${GGO_C} ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS})
target_include_directories(smi PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(smi sm-static)

target_compile_options(smi PRIVATE -Werror -Wall -pedantic)

include(GNUInstallDirs)
install(TARGETS smi
    EXPORT smi-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# rgression test from child build directory:
# ./smi/smi -d ../smi/test_definition.txt -i ../smi/test_input.txt | diff ../smi/test_output.txt -

# generate cmdline.c/.h
# gengetopt < smi.ggo

