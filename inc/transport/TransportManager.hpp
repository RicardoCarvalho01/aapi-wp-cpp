#pragma once

#include "transport/ITransport.hpp"


namespace aa::transport
{

class TransportManager
{
    public:
        TransportManager(ITransport &usb, ITransport &wifi, ITransport &bluetooth);

        void start();

        void stop();

        [[nodiscard]]
        bool usbConnected() const noexcept;

        [[nodiscard]]
        bool wifiConnected() const noexcept;

        [[nodiscard]]
        bool bluetoothConnected() const noexcept;

        void sendToUsb(Packet const &packet);

        void sendToWifi(Packet const &packet);

        void sendToBluetooth(Packet const &packet);

    private:
        ITransport &usb_;
        ITransport &wifi_;
        ITransport &bluetooth_;
};

}