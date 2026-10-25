#pragma once

#include <cstddef>
#include <cstdint>


namespace aa::core
{

// One entry per state of the Android Auto session handshake. The numeric
// value is the row of the state in the state table, so Count must stay last.
//
// "Start*" states are transient: their entry function does some work and
// immediately returns a transition to the matching "Waiting*" state.
enum class StateId : std::uint8_t
{
    Disconnected = 0,

    StartWifi,
    WaitingWifi,

    StartTransport,
    WaitingTransport,

    StartProtocol,
    WaitingProtocol,

    StartAuthentication,
    WaitingAuthentication,

    StartServices,
    WaitingServices,

    Running,
    Recovery,

    Count
};

inline constexpr std::size_t StateCount = static_cast<std::size_t>(StateId::Count);

[[nodiscard]]
constexpr std::size_t toIndex(StateId id) noexcept
{
    return static_cast<std::size_t>(id);
}

}
