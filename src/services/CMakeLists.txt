
add_library(aapiwp-services INTERFACE)

target_include_directories(aapiwp-services
    INTERFACE
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-services
    INTERFACE
        aapiwp-core
)

target_compile_features(aapiwp-services
    INTERFACE
        cxx_std_23
)
