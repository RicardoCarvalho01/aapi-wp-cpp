#pragma once

#include <cstddef>
#include <type_traits>
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
//
// The position of an alternative in this list is also its column in the
// state machine's transition table (see core/state/StateTable.hpp), so
// adding an event here automatically widens every table row.
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

// Number of distinct event types == number of columns in a state table.
inline constexpr std::size_t EventCount = std::variant_size_v<Event>;

namespace detail
{

template <typename T, typename Variant>
struct VariantIndex;

template <typename T, typename... Ts>
struct VariantIndex<T, std::variant<Ts...>>
{
    static constexpr std::size_t value = []() constexpr
    {
        std::size_t index = 0;

        // Counts alternatives until the first one that is exactly T.
        // (The fold short-circuits on `false`, i.e. on the match.)
        [[maybe_unused]] bool const found =
            ((std::is_same_v<T, Ts> ? false : (++index, true)) && ...);

        return index;
    }();

    static_assert(value < sizeof...(Ts), "type is not an alternative of the variant");
};

}

// Column of event type E in the state table. Evaluated at compile time;
// fails to compile if E is not one of the Event alternatives.
template <typename E>
inline constexpr std::size_t EventIndex = detail::VariantIndex<E, Event>::value;

}
