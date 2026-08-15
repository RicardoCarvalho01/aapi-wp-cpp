#pragma once

#include "core/events/EventQueue.hpp"


namespace aa::drivers::ble
{

class BleAdapter
{
    public:
        explicit BleAdapter(aa::core::EventQueue &eventQueue);

        virtual ~BleAdapter() = default;

        BleAdapter(BleAdapter const &) = delete;
        BleAdapter & operator=(BleAdapter const &) = delete;

        BleAdapter(BleAdapter &&) = delete;
        BleAdapter & operator=(BleAdapter &&) = delete;

        virtual bool initialize() = 0;

        virtual bool startAdvertising() = 0;

        virtual void stopAdvertising() = 0;

        /*
         * Process pending BLE activity and enqueue corresponding
         * application events.
         *
         * poll() is intentionally non-blocking. The application owns
         * the main loop and decides how frequently this is called.
        */
        virtual void poll() = 0;

    protected:
        void enqueue(aa::core::Event event);

        aa::core::EventQueue & eventQueue_;
};

} // namespace aa::drivers::ble