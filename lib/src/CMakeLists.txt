find_package(Threads REQUIRED)



configure_file(version.c.in version.c)

file(GLOB sources *.c )
list(APPEND sources ${CMAKE_CURRENT_BINARY_DIR}/version.c)


add_library(sm-shared SHARED ${sources})
add_library(sm-static STATIC ${sources})


target_link_libraries(sm-shared PRIVATE Threads::Threads)

target_include_directories(sm-shared        
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(sm-static
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(sm-shared PROPERTIES OUTPUT_NAME sm PUBLIC_HEADER $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/sm>)
set_target_properties(sm-static PROPERTIES OUTPUT_NAME sm)


target_compile_options(sm-shared PRIVATE -Werror -Wall -pedantic)
target_compile_options(sm-static PRIVATE -Werror -Wall -pedantic)

include(GNUInstallDirs)
install(TARGETS sm-shared sm-static
    EXPORT sm-export
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)



