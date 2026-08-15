#pragma once

#include <chrono>

#include "core/events/Event.hpp"


namespace aa::core
{

class TimeoutEvent final : public Event
{
    public:
        explicit TimeoutEvent(std::chrono::milliseconds timeout)
            : Event(EventId::Timeout)
            , timeout_(timeout)
        { }

        [[nodiscard]]
        std::chrono::milliseconds timeout() const
        {
            return timeout_;
        }

    private:
        std::chrono::milliseconds timeout_;
};

}