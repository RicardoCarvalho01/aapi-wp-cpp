cmake_minimum_required(VERSION 3.22)

project(
    AndroidAutoPi
    VERSION 0.1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(CompilerOptions)
include(Warnings)
include(Sanitizers)

option(BUILD_TESTING "Build unit tests" ON)
option(BUILD_LINUX_DRIVERS "Build Linux/DBus drivers" OFF)

add_subdirectory(src/core)
add_subdirectory(src/services)
add_subdirectory(src/session)

if(BUILD_LINUX_DRIVERS)
    add_subdirectory(src/drivers/linux)
endif()

add_subdirectory(src/drivers/ble)
add_subdirectory(src/app)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()