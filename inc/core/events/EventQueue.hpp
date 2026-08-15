#pragma once

#include <deque>

#include "core/events/Event.hpp"


namespace aa::core
{

class EventQueue
{
    public:
        void push(Event event);

        [[nodiscard]]
        Event pop();

        [[nodiscard]]
        bool empty() const noexcept;

        [[nodiscard]]
        std::size_t size() const noexcept;

        void clear();

    private:
        std::deque<Event> queue_;
};

}