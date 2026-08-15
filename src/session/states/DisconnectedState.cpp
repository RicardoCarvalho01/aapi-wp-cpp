
#include "session/states/DisconnectedState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void DisconnectedState::onEvent(aa::core::StateMachine& machine, const aa::core::BleConnectedEvent&)
{
    machine.transitionTo(machine.context().startWifiState());
}

} // namespace aa::session
