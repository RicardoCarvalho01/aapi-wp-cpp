
#include "drivers/linux/LinuxTransport.hpp"

namespace aa::drivers::linux
{

void LinuxTransport::open()
{
    opened_ = true;
}

void LinuxTransport::close()
{
    opened_ = false;
}

void LinuxTransport::send(std::span<const std::byte> data)
{
    (void) data;

    if (!opened_)
    {
        return;
    }

    // TODO:
    // USB bulk endpoint
    // TCP socket
    // AA transport layer
}

bool LinuxTransport::opened() const
{
    return opened_;
}

} // namespace aa::drivers::linux