
#include "drivers/ble/BleAdapter.hpp"

#include <utility>

namespace aa::drivers::ble
{

BleAdapter::BleAdapter(aa::core::EventQueue& eventQueue) : eventQueue_(eventQueue)
{}

void BleAdapter::enqueue(aa::core::Event event)
{
    eventQueue_.push(std::move(event));
}

} // namespace aa::drivers::ble