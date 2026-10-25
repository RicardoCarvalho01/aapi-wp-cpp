#pragma once

#include "core/events/EventFlags.hpp"


namespace aa::drivers::ble
{

class BleAdapter
{
    public:
        explicit BleAdapter(aa::core::EventFlags &events) noexcept;

        virtual ~BleAdapter() = default;

        BleAdapter(BleAdapter const &) = delete;
        BleAdapter & operator=(BleAdapter const &) = delete;

        BleAdapter(BleAdapter &&) = delete;
        BleAdapter & operator=(BleAdapter &&) = delete;

        virtual bool initialize() = 0;

        virtual bool startAdvertising() = 0;

        virtual void stopAdvertising() = 0;

        /*
         * Process pending BLE activity and raise the corresponding
         * event flags.
         *
         * poll() is intentionally non-blocking. The application decides
         * how frequently this is called.
        */
        virtual void poll() = 0;

    protected:
        void raise(aa::core::EventFlag flag) noexcept;

        aa::core::EventFlags & events_;
};

} // namespace aa::drivers::ble