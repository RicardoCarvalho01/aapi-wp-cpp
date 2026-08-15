#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Waiting for the application services started by StartServicesState to
// report they are ready, at which point the session is fully up.
class WaitingServicesState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::ServicesReadyEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::WifiDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
