
#include "core/events/EventDispatcher.hpp"

#include "core/state/StateMachine.hpp"

namespace aa::core
{

EventDispatcher::EventDispatcher(EventQueue& queue, StateMachine& machine)
    : queue_(queue), machine_(machine)
{}

void EventDispatcher::dispatch()
{
    if (queue_.empty())
    {
        return;
    }

    machine_.dispatch(queue_.pop());
}

void EventDispatcher::dispatchAll()
{
    while (!queue_.empty())
    {
        machine_.dispatch(queue_.pop());
    }
}

} // namespace aa::core
