#pragma once

#include <atomic>

#include "services/IWifi.hpp"


namespace aa::drivers::linux
{

class LinuxWifi final : public aa::services::IWifi
{
    public:
        void start() override;

        void stop() override;

        void connect() override;

        void disconnect() override;

        [[nodiscard]]
        bool connected() const override;

    private:
        std::atomic_bool started_{false};
        std::atomic_bool connected_{false};
};

} // namespace aa::drivers::linux