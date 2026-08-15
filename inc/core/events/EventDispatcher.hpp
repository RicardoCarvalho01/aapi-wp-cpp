#pragma once

#include "core/events/EventQueue.hpp"


namespace aa::core
{

class StateMachine;

class EventDispatcher
{
    public:
        EventDispatcher(EventQueue &queue, StateMachine &machine);

        void dispatch();

        void dispatchAll();

    private:
        EventQueue &queue_;

        StateMachine &machine_;
};

}