
#include "session/states/RunningState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void RunningState::onEvent(aa::core::StateMachine& machine, const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void RunningState::onEvent(aa::core::StateMachine& machine, const aa::core::WifiDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void RunningState::onEvent(aa::core::StateMachine& machine, const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
