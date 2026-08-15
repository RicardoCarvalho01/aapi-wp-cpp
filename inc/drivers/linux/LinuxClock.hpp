#pragma once

#include "services/IClock.hpp"


namespace aa::drivers::linux
{

class LinuxClock final : public aa::services::IClock
{
    public:
        [[nodiscard]]
        std::chrono::steady_clock::time_point now() const override;
};

}