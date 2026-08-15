#pragma once

#include <cstdint>


namespace aa::core
{

enum class EventType : std::uint8_t
{
    None = 0,

    BleConnected,
    BleDisconnected,

    WifiConnected,
    WifiDisconnected,

    UsbAttached,
    UsbDetached,

    PairingRequest,
    PairingCompleted,

    TransportReady,

    Error
};

} // namespace aa::core