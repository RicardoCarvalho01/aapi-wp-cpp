
#include "drivers/linux/LinuxProtocol.hpp"

namespace aa::drivers::linux
{

void LinuxProtocol::start()
{
    running_ = true;
}

void LinuxProtocol::stop()
{
    running_ = false;
}

void LinuxProtocol::reset()
{
    running_ = false;
    running_ = true;
}

bool LinuxProtocol::running() const
{
    return running_;
}

} // namespace aa::drivers::linux