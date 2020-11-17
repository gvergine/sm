cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)

project(
    sm
    VERSION 1.0
    LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


add_subdirectory(lib)
add_subdirectory(smi)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Giovanni Vergine <verginegiovanni@gmail.com>") #required
INCLUDE(CPack)
