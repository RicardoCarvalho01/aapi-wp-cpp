
#include "session/states/WaitingServicesState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void WaitingServicesState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::ServicesReadyEvent&)
{
    machine.transitionTo(machine.context().runningState());
}

void WaitingServicesState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingServicesState::onEvent(aa::core::StateMachine& machine,
                                   const aa::core::WifiDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingServicesState::onEvent(aa::core::StateMachine& machine, const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
