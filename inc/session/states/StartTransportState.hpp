#pragma once

#include "core/state/State.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// Transient: opens the transport (USB or wifi-backed) and immediately
// hands off to WaitingTransportState.
class StartTransportState final : public aa::core::State
{
    public:
        void setServices(aa::services::IServices *services) noexcept
        {
            services_ = services;
        }

        void onEnter(aa::core::StateMachine &machine) override;

    private:
        aa::services::IServices *services_{nullptr};
};

}
