
#include "session/states/RecoveryState.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::session
{

void RecoveryState::onEnter(aa::core::StateMachine& machine)
{
    if (services_ != nullptr)
    {
        // Order matters: unwind the handshake in the reverse order it was
        // built up in, highest layer first, so each driver is torn down
        // while the layer below it is still available if it needs it.
        services_->bluetooth().unpair();
        services_->bluetooth().stop();

        services_->protocol().stop();

        services_->transport().close();

        services_->wifi().disconnect();
        services_->wifi().stop();
    }

    machine.transitionTo(machine.context().disconnectedState());
}

} // namespace aa::session
