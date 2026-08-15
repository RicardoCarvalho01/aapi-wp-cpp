
#include "session/states/StartProtocolState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void StartProtocolState::onEnter(aa::core::StateMachine& machine)
{
    if (services_ != nullptr)
    {
        services_->protocol().start();
    }

    machine.transitionTo(machine.context().waitingProtocolState());
}

} // namespace aa::session
