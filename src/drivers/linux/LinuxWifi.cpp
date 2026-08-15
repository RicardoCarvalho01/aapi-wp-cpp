
#include "drivers/linux/LinuxWifi.hpp"

namespace aa::drivers::linux
{

void LinuxWifi::start()
{
    started_ = true;
}

void LinuxWifi::stop()
{
    connected_ = false;
    started_ = false;
}

void LinuxWifi::connect()
{
    if (!started_)
    {
        return;
    }

    connected_ = true;
}

void LinuxWifi::disconnect()
{
    connected_ = false;
}

bool LinuxWifi::connected() const
{
    return connected_;
}

} // namespace aa::drivers::linux