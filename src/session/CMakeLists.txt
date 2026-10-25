
add_library(aapiwp-session
    Session.cpp
    SessionTable.cpp
)

target_include_directories(aapiwp-session
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-session
    PUBLIC
        aapiwp-core
        aapiwp-services
        Threads::Threads
)

target_compile_features(aapiwp-session
    PUBLIC
        cxx_std_23
)
