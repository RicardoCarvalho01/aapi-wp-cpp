#pragma once

#include "core/state/State.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// Transient: tears down whatever was set up so far and immediately hands
// off to DisconnectedState, so the session can start the handshake over
// from a clean slate.
class RecoveryState final : public aa::core::State
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
