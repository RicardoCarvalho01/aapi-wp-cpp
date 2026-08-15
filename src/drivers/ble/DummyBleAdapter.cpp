
#include "drivers/ble/DummyBleAdapter.hpp"

#include "core/events/BleConnectedEvent.hpp"
#include "core/events/BleDisconnectedEvent.hpp"

namespace aa::drivers::ble
{

DummyBleAdapter::DummyBleAdapter(aa::core::EventQueue& eventQueue) : BleAdapter(eventQueue)
{}

bool DummyBleAdapter::initialize()
{
    initialized_ = true;
    return true;
}

bool DummyBleAdapter::startAdvertising()
{
    if (!initialized_)
    {
        return false;
    }

    advertising_ = true;
    return true;
}

void DummyBleAdapter::stopAdvertising()
{
    advertising_ = false;
}

void DummyBleAdapter::poll()
{
    // The dummy adapter has no external I/O source.
}

bool DummyBleAdapter::initialized() const noexcept
{
    return initialized_;
}

bool DummyBleAdapter::advertising() const noexcept
{
    return advertising_;
}

bool DummyBleAdapter::connected() const noexcept
{
    return connected_;
}

void DummyBleAdapter::simulateConnection()
{
    if (!initialized_ || !advertising_ || connected_)
    {
        return;
    }

    connected_ = true;
    advertising_ = false;

    eventQueue_.push(aa::core::BleConnectedEvent{});
}

void DummyBleAdapter::simulateDisconnection()
{
    if (!connected_)
    {
        return;
    }

    connected_ = false;
    eventQueue_.push(aa::core::BleDisconnectedEvent{});
}

} // namespace aa::drivers::ble