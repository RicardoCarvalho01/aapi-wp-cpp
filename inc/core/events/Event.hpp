#pragma once

#include <variant>

#include "core/events/BleConnectedEvent.hpp"
#include "core/events/BleDisconnectedEvent.hpp"
#include "core/events/ErrorEvent.hpp"
#include "core/events/PairingCompletedEvent.hpp"
#include "core/events/PairingRequestEvent.hpp"
#include "core/events/ServicesReadyEvent.hpp"
#include "core/events/TransportReadyEvent.hpp"
#include "core/events/WifiConnectedEvent.hpp"
#include "core/events/WifiDisconnectedEvent.hpp"


namespace aa::core
{

// The set of signals the session state machine reacts to. Each alternative
// is a small, independent struct (see core/events/*.hpp) so new signals can
// be added without touching this list's users, only this variant.
using Event = std::variant<
        BleConnectedEvent,
        BleDisconnectedEvent,
        WifiConnectedEvent,
        WifiDisconnectedEvent,
        TransportReadyEvent,
        PairingRequestEvent,
        PairingCompletedEvent,
        ServicesReadyEvent,
        ErrorEvent
    >;

}
