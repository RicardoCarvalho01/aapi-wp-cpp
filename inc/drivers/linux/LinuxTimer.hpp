#pragma once

#include <atomic>
#include <chrono>

#include "services/ITimer.hpp"


namespace aa::drivers::linux
{

class LinuxTimer final : public aa::services::ITimer
{
    public:
        void start(std::chrono::milliseconds timeout) override;

        void stop() override;

        [[nodiscard]]
        bool running() const override;

    private:
        std::atomic_bool running_{false};
        std::chrono::milliseconds timeout_{};
};

}