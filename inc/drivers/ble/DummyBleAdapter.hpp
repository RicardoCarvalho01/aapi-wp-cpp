#pragma once

#include "drivers/ble/BleAdapter.hpp"


namespace aa::drivers::ble
{

class DummyBleAdapter final : public BleAdapter
{
    public:
        explicit DummyBleAdapter(aa::core::EventQueue &eventQueue);

        bool initialize() override;

        bool startAdvertising() override;

        void stopAdvertising() override;

        void poll() override;

        [[nodiscard]]
        bool initialized() const noexcept;

        [[nodiscard]]
        bool advertising() const noexcept;

        [[nodiscard]]
        bool connected() const noexcept;

        void simulateConnection();

        void simulateDisconnection();

    private:
        bool initialized_{false};
        bool advertising_{false};
        bool connected_{false};
};

} // namespace aa::drivers::ble