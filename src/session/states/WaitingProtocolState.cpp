
#include "session/states/WaitingProtocolState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void WaitingProtocolState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::PairingRequestEvent&)
{
    machine.transitionTo(machine.context().startAuthenticationState());
}

void WaitingProtocolState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingProtocolState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::WifiDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingProtocolState::onEvent(aa::core::StateMachine& machine, const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
