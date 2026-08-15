
#include "drivers/linux/LinuxTimer.hpp"

namespace aa::drivers::linux
{

void LinuxTimer::start(std::chrono::milliseconds timeout)
{
    timeout_ = timeout;
    running_ = true;
}

void LinuxTimer::stop()
{
    running_ = false;
}

bool LinuxTimer::running() const
{
    return running_;
}

} // namespace aa::drivers::linux