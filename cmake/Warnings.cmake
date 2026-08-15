# Warnings.cmake

add_library(project_warnings INTERFACE)

if(MSVC)

    target_compile_options(
        project_warnings
        INTERFACE
            /W4
            /WX
            /permissive-
    )

else()

    target_compile_options(
        project_warnings
        INTERFACE

            -Wall
            -Wextra
            -Wpedantic

            -Wshadow
            -Wconversion
            -Wsign-conversion

            -Wcast-align
            -Wcast-qual

            -Wnull-dereference

            -Wdouble-promotion

            -Wimplicit-fallthrough

            -Werror
    )

endif()