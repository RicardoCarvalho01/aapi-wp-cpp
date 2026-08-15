#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// The session is fully established: all Android Auto services are up and
// running. Any disconnect or error sends the session through recovery.
class RunningState final : public aa::core::State
{
    public:
        void onEvent(aa::core::StateMachine &machine, aa::core::BleDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::WifiDisconnectedEvent const &event) override;

        void onEvent(aa::core::StateMachine &machine, aa::core::ErrorEvent const &event) override;
};

}
