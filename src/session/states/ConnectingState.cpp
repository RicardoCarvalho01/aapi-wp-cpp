
#include "session/states/ConnectingState.hpp"

#include "bridge/Bridge.hpp"
#include "core/events/EventId.hpp"
#include "core/state/IStateContext.hpp"
#include "core/state/StateMachine.hpp"

namespace aa::session
{

void ConnectingState::onEnter(aa::core::StateMachine& machine)
{
    machine.context().bridge().start();
}

aa::core::Transition ConnectingState::handleEvent(aa::core::StateMachine&,
                                                  const aa::core::Event& event)
{
    switch (event.id())
    {
        case aa::core::EventId::NegotiationSucceeded:
            return go(aa::core::StateId::Running);

        case aa::core::EventId::NegotiationFailed:
            return go(aa::core::StateId::Idle);

        case aa::core::EventId::UsbDetached:
            return go(aa::core::StateId::Idle);

        default:
            return stay();
    }
}

} // namespace aa::session