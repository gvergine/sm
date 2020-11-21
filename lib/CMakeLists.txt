add_subdirectory(src)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_subdirectory(tests)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
