#pragma once

#include "core/state/State.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// Transient: starts the pairing/authentication exchange requested by the
// far end and immediately hands off to WaitingAuthenticationState.
class StartAuthenticationState final : public aa::core::State
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
