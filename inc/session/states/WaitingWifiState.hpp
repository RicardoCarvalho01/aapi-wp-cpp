#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Waiting for the wifi link established by StartWifiState to come up.
class WaitingWifiState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::WifiConnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
