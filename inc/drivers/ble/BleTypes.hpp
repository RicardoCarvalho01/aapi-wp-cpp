#pragma once

#include <array>
#include <cstdint>
#include <span>


namespace aa::drivers::ble
{

using Uuid16 = std::uint16_t;

using Uuid128 = std::array<std::uint8_t, 16>;

using Buffer = std::span<std::uint8_t>;

using ConstBuffer = std::span<std::uint8_t const>;

} // namespace aa::drivers::ble