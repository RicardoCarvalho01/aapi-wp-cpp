#pragma once

#include "core/events/Event.hpp"


namespace aa::core
{

class UsbAttachedEvent final : public Event
{
    public:
        UsbAttachedEvent()
            : Event(EventId::UsbAttached)
        { }
};

}