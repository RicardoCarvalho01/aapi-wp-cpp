#pragma once

#include "drivers/linux/LinuxBluetooth.hpp"
#include "drivers/linux/LinuxClock.hpp"
#include "drivers/linux/LinuxLogger.hpp"
#include "drivers/linux/LinuxProtocol.hpp"
#include "drivers/linux/LinuxTimer.hpp"
#include "drivers/linux/LinuxTransport.hpp"
#include "drivers/linux/LinuxUsb.hpp"
#include "drivers/linux/LinuxWifi.hpp"

#include "services/IServices.hpp"


namespace aa::drivers::linux
{

class LinuxServices final : public aa::services::IServices
{
    public:
        [[nodiscard]]
        aa::services::ILogger & logger() override;

        [[nodiscard]]
        aa::services::ITimer & timer() override;

        [[nodiscard]]
        aa::services::IClock & clock() override;

        [[nodiscard]]
        aa::services::IUsb & usb() override;

        [[nodiscard]]
        aa::services::IWifi & wifi() override;

        [[nodiscard]]
        aa::services::IBluetooth & bluetooth() override;

        [[nodiscard]]
        aa::services::ITransport & transport() override;

        [[nodiscard]]
        aa::services::IProtocol & protocol() override;

    private:
        LinuxLogger logger_;
        LinuxTimer timer_;
        LinuxClock clock_;
        LinuxUsb usb_;
        LinuxWifi wifi_;
        LinuxBluetooth bluetooth_;
        LinuxTransport transport_;
        LinuxProtocol protocol_;
};

} // namespace aa::drivers::linux