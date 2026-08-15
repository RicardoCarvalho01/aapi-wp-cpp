
#include "session/states/IdleState.hpp"

#include "bridge/Bridge.hpp"
#include "core/events/EventId.hpp"
#include "core/state/IStateContext.hpp"
#include "core/state/StateMachine.hpp"
#include "usb/Usb.hpp"
#include "wireless/Wireless.hpp"

namespace aa::session
{

void IdleState::onEnter(aa::core::StateMachine& machine)
{
    auto& context = machine.context();

    context.usb().start();
    context.wireless().start();
}

aa::core::Transition IdleState::handleEvent(aa::core::StateMachine& /*unused*/,
                                            const aa::core::Event& event)
{
    switch (event.id())
    {
        case aa::core::EventId::UsbAttached:
            return go(aa::core::StateId::Connecting);

        default:
            return stay();
    }
}

} // namespace aa::session