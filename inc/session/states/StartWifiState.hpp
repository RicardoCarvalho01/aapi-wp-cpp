#pragma once

#include "core/state/State.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// Transient: kicks off the wifi connection attempt and immediately hands
// off to WaitingWifiState, which is where the session actually waits for
// the wifi link to come up.
class StartWifiState final : public aa::core::State
{
    public:
        // Optional: without a services pointer (e.g. in isolated state
        // unit tests) onEnter() simply skips the driver call and still
        // transitions, which keeps this state default-constructible.
        void setServices(aa::services::IServices *services) noexcept
        {
            services_ = services;
        }

        void onEnter(aa::core::StateMachine &machine) override;

    private:
        aa::services::IServices *services_{nullptr};
};

}
