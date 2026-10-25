
#include "drivers/ble/DummyBleAdapter.hpp"

namespace aa::drivers::ble
{

DummyBleAdapter::DummyBleAdapter(aa::core::EventFlags& events) noexcept : BleAdapter(events)
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

    raise(aa::core::EventFlag::BleConnected);
}

void DummyBleAdapter::simulateDisconnection()
{
    if (!connected_)
    {
        return;
    }

    connected_ = false;
    raise(aa::core::EventFlag::BleDisconnected);
}

} // namespace aa::drivers::ble