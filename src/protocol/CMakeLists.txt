
add_library(aapiwp-protocol)

target_include_directories(aapiwp-protocol
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-protocol
    PUBLIC
        aapiwp-core
        project_warnings
        project_sanitizers
)

target_compile_features(aapiwp-protocol
    PUBLIC
        cxx_std_23
)
