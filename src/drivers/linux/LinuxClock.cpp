
#include "drivers/linux/LinuxClock.hpp"

namespace aa::drivers::linux
{

std::chrono::steady_clock::time_point LinuxClock::now() const
{
    return std::chrono::steady_clock::now();
}

} // namespace aa::drivers::linux