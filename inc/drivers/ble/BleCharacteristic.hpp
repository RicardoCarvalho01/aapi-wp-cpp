#pragma once

#include "drivers/ble/BleTypes.hpp"


namespace aa::drivers::ble
{

class BleCharacteristic
{
    public:
        virtual ~BleCharacteristic() = default;

        BleCharacteristic(BleCharacteristic const &) = delete;
        BleCharacteristic & operator=(BleCharacteristic const &) = delete;

        BleCharacteristic(BleCharacteristic &&) = delete;
        BleCharacteristic & operator=(BleCharacteristic &&) = delete;

        [[nodiscard]]
        virtual Uuid128 uuid() const = 0;

        virtual bool notify(ConstBuffer data) = 0;

    protected:
        BleCharacteristic() = default;
};

} // namespace aa::drivers::ble