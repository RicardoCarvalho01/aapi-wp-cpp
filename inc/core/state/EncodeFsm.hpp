#pragma once

#include "core/events/EventFlags.hpp"
#include "core/state/StateId.hpp"
#include "core/state/StateTable.hpp"
#include "core/state/Transition.hpp"


namespace aa::core
{

// One step of the state machine. Non-blocking: call it periodically (from a
// timer) and it returns the state to be current on the next call.
//
//   1. drop pending events that make no sense in `current`;
//   2. look `current` up in the table and run its action;
//   3. pick the next state: the automatic one (transient state), else the
//      one the action asked for, else stay.
//
// There is no dispatch and no queue: the action looks at the flags itself.
template <typename Context>
[[nodiscard]]
StateId encodeFSM(StateTable<Context> const &table,
                  StateId current,
                  Context &context,
                  EventFlags &events)
{
    StateRow<Context> const &row = table[toIndex(current)];

    // Stale events: raised, but irrelevant to this state. Dropped here;
    // clearExcept() returns the mask if you want to log/evaluate them.
    (void) events.clearExcept(row.accepts);

    Transition const requested =
        row.action != nullptr ? row.action(context, events) : Transition::stay();

    if (row.then.valid())
    {
        return row.then.target();
    }

    return requested.valid() ? requested.target() : current;
}

}
