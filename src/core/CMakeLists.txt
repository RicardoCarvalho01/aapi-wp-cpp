# Header-only: events, state table and encodeFSM live in inc/core.
add_library(aapiwp-core INTERFACE)

target_include_directories(aapiwp-core
    INTERFACE
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-core
    INTERFACE
        project_warnings
        project_sanitizers
)

target_compile_features(aapiwp-core
    INTERFACE
        cxx_std_23
)
