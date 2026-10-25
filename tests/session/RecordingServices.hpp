#pragma once

#include <chrono>
#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "services/IServices.hpp"

namespace aa::test
{

// IServices fake that appends "<driver>.<call>" to a shared log, so tests
// can assert exactly which driver calls a state function made, in order.
class RecordingServices final : public aa::services::IServices
{
public:
    std::vector<std::string> calls;

    aa::services::ILogger& logger() override { return logger_; }
    aa::services::ITimer& timer() override { return timer_; }
    aa::services::IClock& clock() override { return clock_; }
    aa::services::IUsb& usb() override { return usb_; }
    aa::services::IWifi& wifi() override { return wifi_; }
    aa::services::IBluetooth& bluetooth() override { return bluetooth_; }
    aa::services::ITransport& transport() override { return transport_; }
    aa::services::IProtocol& protocol() override { return protocol_; }

private:
    struct Logger final : aa::services::ILogger
    {
        void trace(std::string_view) override {}
        void debug(std::string_view) override {}
        void info(std::string_view) override {}
        void warning(std::string_view) override {}
        void error(std::string_view) override {}
    };

    struct Timer final : aa::services::ITimer
    {
        void start(std::chrono::milliseconds) override {}
        void stop() override {}
        bool running() const override { return false; }
    };

    struct Clock final : aa::services::IClock
    {
        std::chrono::steady_clock::time_point now() const override { return {}; }
    };

    struct Usb final : aa::services::IUsb
    {
        void start() override {}
        void stop() override {}
        bool connected() const override { return false; }
    };

    struct Wifi final : aa::services::IWifi
    {
        explicit Wifi(std::vector<std::string>& log) : log_(log) {}
        void start() override { log_.push_back("wifi.start"); }
        void stop() override { log_.push_back("wifi.stop"); }
        void connect() override { log_.push_back("wifi.connect"); }
        void disconnect() override { log_.push_back("wifi.disconnect"); }
        bool connected() const override { return false; }
        std::vector<std::string>& log_;
    };

    struct Bluetooth final : aa::services::IBluetooth
    {
        explicit Bluetooth(std::vector<std::string>& log) : log_(log) {}
        void start() override { log_.push_back("bluetooth.start"); }
        void stop() override { log_.push_back("bluetooth.stop"); }
        void pair() override { log_.push_back("bluetooth.pair"); }
        void unpair() override { log_.push_back("bluetooth.unpair"); }
        bool paired() const override { return false; }
        std::vector<std::string>& log_;
    };

    struct Transport final : aa::services::ITransport
    {
        explicit Transport(std::vector<std::string>& log) : log_(log) {}
        void open() override { log_.push_back("transport.open"); }
        void close() override { log_.push_back("transport.close"); }
        void send(std::span<std::byte const>) override {}
        bool opened() const override { return false; }
        std::vector<std::string>& log_;
    };

    struct Protocol final : aa::services::IProtocol
    {
        explicit Protocol(std::vector<std::string>& log) : log_(log) {}
        void start() override { log_.push_back("protocol.start"); }
        void stop() override { log_.push_back("protocol.stop"); }
        void reset() override {}
        bool running() const override { return false; }
        std::vector<std::string>& log_;
    };

    Logger logger_;
    Timer timer_;
    Clock clock_;
    Usb usb_;
    Wifi wifi_{calls};
    Bluetooth bluetooth_{calls};
    Transport transport_{calls};
    Protocol protocol_{calls};
};

} // namespace aa::test
