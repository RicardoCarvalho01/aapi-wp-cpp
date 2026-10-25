
#include "drivers/ble/BleAdapter.hpp"

namespace aa::drivers::ble
{

BleAdapter::BleAdapter(aa::core::EventFlags& events) noexcept : events_(events)
{}

void BleAdapter::raise(aa::core::EventFlag flag) noexcept
{
    events_.set(flag);
}

} // namespace aa::drivers::ble