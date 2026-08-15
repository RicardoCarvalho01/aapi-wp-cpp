
#include "drivers/linux/LinuxServices.hpp"

namespace aa::drivers::linux
{

aa::services::ILogger& LinuxServices::logger()
{
    return logger_;
}

aa::services::ITimer& LinuxServices::timer()
{
    return timer_;
}

aa::services::IClock& LinuxServices::clock()
{
    return clock_;
}

aa::services::IUsb& LinuxServices::usb()
{
    return usb_;
}

aa::services::IWifi& LinuxServices::wifi()
{
    return wifi_;
}

aa::services::IBluetooth& LinuxServices::bluetooth()
{
    return bluetooth_;
}

aa::services::ITransport& LinuxServices::transport()
{
    return transport_;
}

aa::services::IProtocol& LinuxServices::protocol()
{
    return protocol_;
}

} // namespace aa::drivers::linux