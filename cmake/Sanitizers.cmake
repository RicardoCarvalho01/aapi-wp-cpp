# Sanitizers.cmake

option(
    ENABLE_SANITIZERS
    "Enable Address/Undefined sanitizers"
    ON
)

add_library(project_sanitizers INTERFACE)

if(
    ENABLE_SANITIZERS
    AND CMAKE_BUILD_TYPE STREQUAL "Debug"
    AND NOT MSVC
)

    target_compile_options(
        project_sanitizers
        INTERFACE

            -fsanitize=address
            -fsanitize=undefined
            -fno-omit-frame-pointer
    )

    target_link_options(
        project_sanitizers
        INTERFACE

            -fsanitize=address
            -fsanitize=undefined
    )

endif()