#pragma once

#include "core/events/EventQueue.hpp"


namespace aa::core
{

// Feeds queued events into a state machine. Works with any machine that
// has `void dispatch(Event const &)` (i.e. StateMachine<Context>).
template <typename Machine>
class EventDispatcher
{
    public:
        EventDispatcher(EventQueue &queue, Machine &machine) noexcept
            : queue_(queue)
            , machine_(machine)
        { }

        // Processes at most one queued event.
        void dispatch()
        {
            if (queue_.empty())
            {
                return;
            }

            machine_.dispatch(queue_.pop());
        }

        // Processes events until the queue is empty (including any that
        // handlers push while running).
        void dispatchAll()
        {
            while (!queue_.empty())
            {
                machine_.dispatch(queue_.pop());
            }
        }

    private:
        EventQueue &queue_;

        Machine &machine_;
};

}
