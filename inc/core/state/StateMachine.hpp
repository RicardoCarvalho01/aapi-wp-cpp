#pragma once

#include <cassert>
#include <cstddef>
#include <optional>

#include "core/events/Event.hpp"
#include "core/state/StateId.hpp"
#include "core/state/StateTable.hpp"
#include "core/state/Transition.hpp"


namespace aa::core
{

// A table-driven state machine.
//
// The machine itself knows nothing about any particular state graph: it
// only tracks the current StateId and looks up what to do in a StateTable
// (a lookup table of function pointers, see StateTable.hpp). The table is
// borrowed, not copied, so it can live in static storage (constexpr/ROM).
//
// `Context` is whatever the handlers need to do their work (drivers,
// services, ...). The machine hands it to every handler untouched.
template <typename Context>
class StateMachine
{
    public:
        using Table = StateTable<Context>;

        StateMachine(Context &context, Table const &table) noexcept
            : context_(context)
            , table_(table)
        { }

        // Makes `initial` the current state and runs its onEnter function
        // (following any transitions it returns). There is no old state,
        // so no onExit runs.
        void start(StateId initial)
        {
            current_ = initial;

            apply(enterCurrent());
        }

        // Looks up the handler for (current state, event type) and runs it.
        // Ignored if the machine has not been started or the table has no
        // handler in that cell.
        void dispatch(Event const &event)
        {
            if (!current_.has_value())
            {
                return;
            }

            auto const handler = table_[toIndex(*current_)].on[event.index()];

            if (handler == nullptr)
            {
                return;
            }

            apply(handler(context_, event));
        }

        // Empty until start() has been called.
        [[nodiscard]]
        std::optional<StateId> currentState() const noexcept
        {
            return current_;
        }

        [[nodiscard]]
        Context & context() noexcept
        {
            return context_;
        }

    private:
        // A chain of transient states can never legitimately be longer than
        // the number of states; anything longer is a loop in the table.
        static constexpr std::size_t MaxChain = StateCount;

        [[nodiscard]]
        Transition enterCurrent()
        {
            auto const enter = table_[toIndex(*current_)].onEnter;

            return enter != nullptr ? enter(context_) : Transition::stay();
        }

        // Performs the transition (exit old, enter new) and keeps following
        // transitions returned by onEnter, so transient states chain
        // iteratively instead of recursing.
        void apply(Transition transition)
        {
            std::size_t hops = 0;

            while (transition.valid())
            {
                if (hops++ >= MaxChain)
                {
                    assert(false && "transient state loop in state table");

                    return;
                }

                if (auto const exit = table_[toIndex(*current_)].onExit; exit != nullptr)
                {
                    exit(context_);
                }

                current_ = transition.target();

                transition = enterCurrent();
            }
        }

        Context &context_;

        Table const &table_;

        std::optional<StateId> current_{};
};

}
