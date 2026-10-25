#pragma once

#include <chrono>
#include <cstddef>
#include <span>
#include <string_view>

#include "services/IServices.hpp"


namespace aa::services
{

// Header-only IServices whose drivers do nothing. It lets the session run
// (and the app link) on a host that has no real drivers, e.g. when
// BUILD_LINUX_DRIVERS is OFF.
class NullServices final : public IServices
{
    public:
        [[nodiscard]]
        ILogger & logger() override { return logger_; }

        [[nodiscard]]
        ITimer & timer() override { return timer_; }

        [[nodiscard]]
        IClock & clock() override { return clock_; }

        [[nodiscard]]
        IUsb & usb() override { return usb_; }

        [[nodiscard]]
        IWifi & wifi() override { return wifi_; }

        [[nodiscard]]
        IBluetooth & bluetooth() override { return bluetooth_; }

        [[nodiscard]]
        ITransport & transport() override { return transport_; }

        [[nodiscard]]
        IProtocol & protocol() override { return protocol_; }

    private:
        struct Logger final : ILogger
        {
            void trace(std::string_view) override { }
            void debug(std::string_view) override { }
            void info(std::string_view) override { }
            void warning(std::string_view) override { }
            void error(std::string_view) override { }
        };

        struct Timer final : ITimer
        {
            void start(std::chrono::milliseconds) override { }
            void stop() override { }
            [[nodiscard]] bool running() const override { return false; }
        };

        struct Clock final : IClock
        {
            [[nodiscard]]
            std::chrono::steady_clock::time_point now() const override
            {
                return std::chrono::steady_clock::now();
            }
        };

        struct Usb final : IUsb
        {
            void start() override { }
            void stop() override { }
            [[nodiscard]] bool connected() const override { return false; }
        };

        struct Wifi final : IWifi
        {
            void start() override { }
            void stop() override { }
            void connect() override { }
            void disconnect() override { }
            [[nodiscard]] bool connected() const override { return false; }
        };

        struct Bluetooth final : IBluetooth
        {
            void start() override { }
            void stop() override { }
            void pair() override { }
            void unpair() override { }
            [[nodiscard]] bool paired() const override { return false; }
        };

        struct Transport final : ITransport
        {
            void open() override { }
            void close() override { }
            void send(std::span<std::byte const>) override { }
            [[nodiscard]] bool opened() const override { return false; }
        };

        struct Protocol final : IProtocol
        {
            void start() override { }
            void stop() override { }
            void reset() override { }
            [[nodiscard]] bool running() const override { return false; }
        };

        Logger logger_;
        Timer timer_;
        Clock clock_;
        Usb usb_;
        Wifi wifi_;
        Bluetooth bluetooth_;
        Transport transport_;
        Protocol protocol_;
};

}
