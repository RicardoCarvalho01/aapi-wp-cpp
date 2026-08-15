#pragma once

#include "core/events/Event.hpp"
#include "core/state/IStateContext.hpp"
#include "core/state/State.hpp"


namespace aa::core
{

// A minimal, context-driven state machine.
//
// It holds no notion of "all states" or a fixed state identifier: it only
// ever tracks the *current* state, and states themselves decide where to go
// next by pulling the target state out of the IStateContext (see
// State::onEnter / State::onEvent implementations in aa::session).
//
// This keeps individual states trivially testable in isolation (a state and
// a MockStateContext are all a test needs) while Session wires the real
// state graph together simply by implementing IStateContext.
class StateMachine
{
    public:
        explicit StateMachine(IStateContext &context) noexcept;

        // Sets the current state without going through transitionTo's
        // onExit step (there is nothing to exit from yet) and calls the
        // new state's onEnter. Transient states may immediately call
        // transitionTo() again from within their own onEnter.
        void setInitialState(State &initial);

        // Exits the current state (if any) and enters `next`.
        void transitionTo(State &next);

        // Routes the event to the current state's matching onEvent
        // overload. A no-op if no initial state has been set yet.
        void dispatch(Event const &event);

        [[nodiscard]]
        State * currentState() const noexcept;

        [[nodiscard]]
        IStateContext & context() noexcept;

    private:
        IStateContext &context_;

        State *currentState_{nullptr};
};

}
