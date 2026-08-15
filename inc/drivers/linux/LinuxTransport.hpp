#pragma once

#include <atomic>
#include <cstddef>
#include <span>

#include "services/ITransport.hpp"


namespace aa::drivers::linux
{

class LinuxTransport final : public aa::services::ITransport
{
    public:
        void open() override;

        void close() override;

        void send(std::span<const std::byte> data) override;

        [[nodiscard]]
        bool opened() const override;

    private:
        std::atomic_bool opened_{false};
};

} // namespace aa::drivers::linux