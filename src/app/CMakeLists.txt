
add_executable(aapiwp
    main.cpp
)

target_link_libraries(aapiwp
    PRIVATE
        aapiwp-core
        aapiwp-services
        aapiwp-session
        aapiwp-ble-drivers
)

if(BUILD_LINUX_DRIVERS)
    target_link_libraries(aapiwp
        PRIVATE
            aapiwp-linux-drivers
    )

    target_compile_definitions(aapiwp
        PRIVATE
            AAPIWP_LINUX_DRIVERS
    )
endif()

install(
    TARGETS aapiwp
    RUNTIME DESTINATION bin
)