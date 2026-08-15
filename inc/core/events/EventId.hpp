#pragma once

#include <cstdint>


namespace aa::core
{

enum class EventId : std::uint16_t
{
    None = 0,

    //
    // USB
    //

    UsbAttached,
    UsbDetached,

    //
    // Wireless
    //

    WifiAvailable,
    WifiConnected,
    WifiDisconnected,

    //
    // Android Auto
    //

    NegotiationSucceeded,
    NegotiationFailed,

    //
    // Bridge
    //

    TransportFailed,

    //
    // Internal
    //

    Shutdown
};

}