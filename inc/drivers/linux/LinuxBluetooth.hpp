#pragma once

#include <atomic>
#include <memory>

#include "drivers/linux/IBluezBackend.hpp"
#include "services/IBluetooth.hpp"


namespace aa::drivers::linux
{

class LinuxBluetooth final : public aa::services::IBluetooth
{
    public:
        LinuxBluetooth();

        explicit LinuxBluetooth(std::unique_ptr<IBluezBackend> backend);

        ~LinuxBluetooth() override = default;

        void start() override;

        void stop() override;

        void pair() override;

        void unpair() override;

        [[nodiscard]]
        bool paired() const override;

        [[nodiscard]]
        bool started() const noexcept;

    private:
        std::unique_ptr<IBluezBackend> backend_;
        std::atomic_bool started_{false};
        std::atomic_bool paired_{false};
};

} // namespace aa::drivers::linux