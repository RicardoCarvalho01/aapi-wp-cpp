
#include "session/states/WaitingAuthenticationState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void WaitingAuthenticationState::onEvent(aa::core::StateMachine& machine,
                                         const aa::core::PairingCompletedEvent&)
{
    machine.transitionTo(machine.context().startServicesState());
}

void WaitingAuthenticationState::onEvent(aa::core::StateMachine& machine,
                                         const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingAuthenticationState::onEvent(aa::core::StateMachine& machine,
                                         const aa::core::WifiDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingAuthenticationState::onEvent(aa::core::StateMachine& machine,
                                         const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
