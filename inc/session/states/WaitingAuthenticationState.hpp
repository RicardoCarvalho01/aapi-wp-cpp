#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Waiting for pairing/authentication to complete.
class WaitingAuthenticationState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::PairingCompletedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::WifiDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
