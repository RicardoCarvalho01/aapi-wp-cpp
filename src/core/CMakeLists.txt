
add_library(aapiwp-core)

target_sources(aapiwp-core
    PRIVATE
        events/EventQueue.cpp
        events/EventDispatcher.cpp
        state/StateMachine.cpp
)

target_include_directories(aapiwp-core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-core
    PUBLIC
        project_warnings
        project_sanitizers
)

target_compile_features(aapiwp-core
    PUBLIC
        cxx_std_23
)