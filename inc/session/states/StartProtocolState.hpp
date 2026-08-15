#pragma once

#include "core/state/State.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// Transient: starts the Android Auto protocol/pairing handshake over the
// now-open transport and immediately hands off to WaitingProtocolState.
class StartProtocolState final : public aa::core::State
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
