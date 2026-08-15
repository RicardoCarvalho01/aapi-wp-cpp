#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Waiting for the far end (the phone, over BLE pairing) to request
// pairing, which kicks off authentication.
class WaitingProtocolState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::PairingRequestEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::WifiDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
