#pragma once

#include "services/IBluetooth.hpp"
#include "services/IClock.hpp"
#include "services/ILogger.hpp"
#include "services/IProtocol.hpp"
#include "services/ITimer.hpp"
#include "services/ITransport.hpp"
#include "services/IUsb.hpp"
#include "services/IWifi.hpp"


namespace aa::services
{

class IServices
{
    public:
        virtual ~IServices() = default;

        [[nodiscard]]
        virtual ILogger & logger() = 0;

        [[nodiscard]]
        virtual ITimer & timer() = 0;

        [[nodiscard]]
        virtual IClock & clock() = 0;

        [[nodiscard]]
        virtual IUsb & usb() = 0;

        [[nodiscard]]
        virtual IWifi & wifi() = 0;

        [[nodiscard]]
        virtual IBluetooth & bluetooth() = 0;

        [[nodiscard]]
        virtual ITransport & transport() = 0;

        [[nodiscard]]
        virtual IProtocol & protocol() = 0;
};

} // namespace aa::services