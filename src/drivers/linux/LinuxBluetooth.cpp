
#include "drivers/linux/LinuxBluetooth.hpp"

#include <utility>

#include "drivers/linux/DbusBluezBackend.hpp"

namespace aa::drivers::linux
{

LinuxBluetooth::LinuxBluetooth() : backend_(std::make_unique<DbusBluezBackend>())
{}

LinuxBluetooth::LinuxBluetooth(std::unique_ptr<IBluezBackend> backend)
    : backend_(std::move(backend))
{}

void LinuxBluetooth::start()
{
    if (backend_ == nullptr)
    {
        return;
    }

    if (started_)
    {
        return;
    }

    if (!backend_->start())
    {
        return;
    }

    started_ = true;
}

void LinuxBluetooth::stop()
{
    if (backend_ != nullptr)
    {
        backend_->stop();
    }

    paired_ = false;
    started_ = false;
}

void LinuxBluetooth::pair()
{
    if (!started_ || (backend_ == nullptr))
    {
        return;
    }

    if (backend_->pair())
    {
        paired_ = true;
    }
}

void LinuxBluetooth::unpair()
{
    if (backend_ != nullptr)
    {
        backend_->unpair();
    }

    paired_ = false;
}

bool LinuxBluetooth::paired() const
{
    return paired_;
}

bool LinuxBluetooth::started() const noexcept
{
    return started_;
}

} // namespace aa::drivers::linux