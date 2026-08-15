
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS_CXX REQUIRED IMPORTED_TARGET dbus-cxx-2.0)

add_library(aapiwp-linux-drivers
    LinuxBluetooth.cpp
    DbusBluezBackend.cpp
    LinuxClock.cpp
    LinuxLogger.cpp
    LinuxProtocol.cpp
    LinuxServices.cpp
    LinuxTimer.cpp
    LinuxTransport.cpp
    LinuxUsb.cpp
    LinuxWifi.cpp
)

target_include_directories(aapiwp-linux-drivers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(aapiwp-linux-drivers
    PUBLIC
        aapiwp-core
        PkgConfig::DBUS_CXX
)

target_compile_features(aapiwp-linux-drivers
    PUBLIC
        cxx_std_23
)