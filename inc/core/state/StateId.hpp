#pragma once

#include <cstdint>


namespace aa::core
{

enum class StateId : std::uint8_t
{
    Idle = 0,
    Connecting,
    Running,

    Count
};

constexpr std::size_t StateCount = static_cast<std::size_t>(StateId::Count);

}