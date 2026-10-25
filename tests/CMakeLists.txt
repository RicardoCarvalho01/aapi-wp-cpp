include(FetchContent)

set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)

get_directory_property(_aa_saved_compile_options COMPILE_OPTIONS)
set_directory_properties(PROPERTIES COMPILE_OPTIONS "")

FetchContent_MakeAvailable(googletest)

set_directory_properties(
    PROPERTIES
    COMPILE_OPTIONS "${_aa_saved_compile_options}"
)

add_executable(aapiwp-tests
    core/EventFlagsTest.cpp
    core/EncodeFsmTest.cpp

    session/SessionTableTest.cpp
    session/SessionTest.cpp

    drivers/ble/DummyBleAdapterTest.cpp
)

target_link_libraries(aapiwp-tests
    PRIVATE
        aapiwp-core
        aapiwp-session
        aapiwp-ble-drivers
        gtest_main
        gmock
)

target_compile_features(aapiwp-tests PRIVATE cxx_std_23)

include(GoogleTest)

if(NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(
        aapiwp-tests
        DISCOVERY_MODE PRE_TEST
    )
endif()