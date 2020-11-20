find_package(Check REQUIRED)
find_package(Threads REQUIRED)

file(GLOB sources *.c)

add_executable(sm-unit-tests ${sources})
target_link_libraries(sm-unit-tests PRIVATE sm-shared ${CHECK_LIBRARIES} subunit Threads::Threads rt m)

add_test(unit-tests sm-unit-tests)

