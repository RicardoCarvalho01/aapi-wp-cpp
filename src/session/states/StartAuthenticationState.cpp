
#include "session/states/StartAuthenticationState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void StartAuthenticationState::onEnter(aa::core::StateMachine& machine)
{
    if (services_ != nullptr)
    {
        services_->bluetooth().start();
        services_->bluetooth().pair();
    }

    machine.transitionTo(machine.context().waitingAuthenticationState());
}

} // namespace aa::session
