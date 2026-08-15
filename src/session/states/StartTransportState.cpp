
#include "session/states/StartTransportState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void StartTransportState::onEnter(aa::core::StateMachine& machine)
{
    if (services_ != nullptr)
    {
        services_->transport().open();
    }

    machine.transitionTo(machine.context().waitingTransportState());
}

} // namespace aa::session
