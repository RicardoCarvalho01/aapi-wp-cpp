#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Waiting for the transport opened by StartTransportState to become ready.
class WaitingTransportState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::TransportReadyEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::WifiDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
