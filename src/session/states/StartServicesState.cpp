
#include "session/states/StartServicesState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void StartServicesState::onEnter(aa::core::StateMachine& machine)
{
    // TODO: start the Android Auto services (video/audio/input channels)
    // once IStateContext exposes the active IServices (or Session wires
    // this up directly).

    machine.transitionTo(machine.context().waitingServicesState());
}

} // namespace aa::session
