#pragma once

#include <atomic>

#include "services/IUsb.hpp"


namespace aa::drivers::linux
{

class LinuxUsb final : public aa::services::IUsb
{
    public:
        void start() override;

        void stop() override;

        [[nodiscard]]
        bool connected() const override;

    private:
        std::atomic_bool connected_{false};
};

}