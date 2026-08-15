
#include "session/states/WaitingTransportState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void WaitingTransportState::onEvent(aa::core::StateMachine& machine,
                                    const aa::core::TransportReadyEvent&)
{
    machine.transitionTo(machine.context().startProtocolState());
}

void WaitingTransportState::onEvent(aa::core::StateMachine& machine,
                                    const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingTransportState::onEvent(aa::core::StateMachine& machine,
                                    const aa::core::WifiDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingTransportState::onEvent(aa::core::StateMachine& machine, const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
