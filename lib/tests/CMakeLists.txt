find_package(Threads REQUIRED)
find_package(Check REQUIRED)

file(GLOB sources *.c)

add_executable(sm-unit-tests ${sources})

target_link_libraries(sm-unit-tests PRIVATE sm-shared ${CHECK_LIBRARIES}
                      subunit Threads::Threads rt m)

add_test(sm-unit-tests sm-unit-tests)
setup_target_for_coverage_lcov(NAME sm_coverage
                              EXECUTABLE sm-unit-tests
                              BASE_DIRECTORY "${PROJECT_BINARY_DIR}/lib/src/"
                              LCOV_ARGS "--rc" "lcov_branch_coverage=1"
                              GENHTML_ARGS "--branch-coverage"
                              EXCLUDE "${PROJECT_BINARY_DIR}/lib/src/version.c"
                              )

