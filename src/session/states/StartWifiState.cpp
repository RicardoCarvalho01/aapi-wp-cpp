
#include "session/states/StartWifiState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void StartWifiState::onEnter(aa::core::StateMachine& machine)
{
    if (services_ != nullptr)
    {
        services_->wifi().start();
        services_->wifi().connect();
    }

    machine.transitionTo(machine.context().waitingWifiState());
}

} // namespace aa::session
