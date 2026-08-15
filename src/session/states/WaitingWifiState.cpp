
#include "session/states/WaitingWifiState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void WaitingWifiState::onEvent(aa::core::StateMachine& machine, const aa::core::WifiConnectedEvent&)
{
    machine.transitionTo(machine.context().startTransportState());
}

void WaitingWifiState::onEvent(aa::core::StateMachine& machine,
                               const aa::core::BleDisconnectedEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

void WaitingWifiState::onEvent(aa::core::StateMachine& machine, const aa::core::ErrorEvent&)
{
    machine.transitionTo(machine.context().recoveryState());
}

} // namespace aa::session
