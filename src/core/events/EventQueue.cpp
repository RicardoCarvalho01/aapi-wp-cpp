
#include "core/events/EventQueue.hpp"

#include <cassert>

namespace aa::core
{

void EventQueue::push(Event event)
{
    queue_.push_back(event);
}

Event EventQueue::pop()
{
    assert(!queue_.empty());

    Event event = queue_.front();
    queue_.pop_front();

    return event;
}

bool EventQueue::empty() const noexcept
{
    return queue_.empty();
}

std::size_t EventQueue::size() const noexcept
{
    return queue_.size();
}

void EventQueue::clear()
{
    queue_.clear();
}

} // namespace aa::core