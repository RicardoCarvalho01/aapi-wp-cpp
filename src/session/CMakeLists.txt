
add_library(aapiwp-session
    Session.cpp
    states/DisconnectedState.cpp
    states/StartWifiState.cpp
    states/WaitingWifiState.cpp
    states/StartTransportState.cpp
    states/WaitingTransportState.cpp
    states/StartProtocolState.cpp
    states/WaitingProtocolState.cpp
    states/StartAuthenticationState.cpp
    states/WaitingAuthenticationState.cpp
    states/StartServicesState.cpp
    states/WaitingServicesState.cpp
    states/RunningState.cpp
    states/RecoveryState.cpp
)

target_include_directories(aapiwp-session
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-session
    PUBLIC
        aapiwp-core
        aapiwp-services
)

if(BUILD_LINUX_DRIVERS)
    target_link_libraries(aapiwp-session
        PRIVATE
            aapiwp-linux-drivers
    )
endif()

target_compile_features(aapiwp-session
    PUBLIC
        cxx_std_23
)