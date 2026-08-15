
add_library(aapiwp-ble-drivers
    BleAdapter.cpp
    DummyBleAdapter.cpp
)

target_include_directories(aapiwp-ble-drivers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-ble-drivers
    PUBLIC
        aapiwp-core
)

target_compile_features(aapiwp-ble-drivers
    PUBLIC
        cxx_std_23
)
