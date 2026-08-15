
#include "drivers/linux/LinuxUsb.hpp"

namespace aa::drivers::linux
{

void LinuxUsb::start()
{
    connected_ = true;
}

void LinuxUsb::stop()
{
    connected_ = false;
}

bool LinuxUsb::connected() const
{
    return connected_;
}

} // namespace aa::drivers::linux