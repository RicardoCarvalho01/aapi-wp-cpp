#pragma once

#include "drivers/ble/BleCharacteristic.hpp"


namespace aa::drivers::ble
{

class BlePeripheral
{
    public:
        virtual ~BlePeripheral() = default;

        BlePeripheral(BlePeripheral const &) = delete;
        BlePeripheral & operator=(BlePeripheral const &) = delete;

        BlePeripheral(BlePeripheral &&) = delete;
        BlePeripheral & operator=(BlePeripheral &&) = delete;

        virtual bool startAdvertising() = 0;

        virtual void stopAdvertising() = 0;

    protected:
        BlePeripheral() = default;
};

} // namespace aa::drivers::ble