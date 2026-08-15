#pragma once

#include "drivers/ble/BleAdapter.hpp"


namespace aa::drivers::ble
{

class LinuxBleAdapter final : public BleAdapter
{
    public:
        explicit LinuxBleAdapter(aa::core::EventQueue &eventQueue);

        ~LinuxBleAdapter() override;

        bool initialize() override;

        bool startAdvertising() override;

        void stopAdvertising() override;

        void poll() override;

    private:
        class Impl;
        Impl *impl_{nullptr};
};

} // namespace aa::drivers::ble