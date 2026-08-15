#pragma once

#include <atomic>

#include "services/IProtocol.hpp"


namespace aa::drivers::linux
{

class LinuxProtocol final : public aa::services::IProtocol
{
    public:
        void start() override;

        void stop() override;

        void reset() override;

        [[nodiscard]]
        bool running() const override;

    private:
        std::atomic_bool running_{false};
};

} // namespace aa::drivers::linux