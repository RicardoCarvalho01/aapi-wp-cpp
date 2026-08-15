#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// The session has no BLE link yet. This is the initial state and the state
// every recovery cycle eventually returns to.
class DisconnectedState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::BleConnectedEvent const &event) override;
};

}
