#pragma once

#include <array>
#include <cstdint>

#include "core/events/EventFlags.hpp"
#include "core/state/StateId.hpp"
#include "core/state/Transition.hpp"


namespace aa::core
{

// One row == one state == one action, run over and over (once per tick)
// for as long as the state is current. An action must never block.
//
//   action    does the state's work and may look at / consume event flags.
//             Returns go(next) to leave, or stay() to run again next tick.
//
//   then      the automatic successor. If valid, the state is transient: it
//             runs once and moves to `then` with no input needed, whatever
//             the action returned.
//
//   accepts   the events that make sense in this state. Any other pending
//             flag is dropped at the start of the tick.
template <typename Context>
struct StateRow
{
    using ActionFn = Transition (*)(Context &, EventFlags &);

    ActionFn action{nullptr};

    Transition then{};

    std::uint32_t accepts{0};
};

// The whole machine: one row per StateId, indexed by toIndex(state).
template <typename Context>
using StateTable = std::array<StateRow<Context>, StateCount>;


// Compile-time helper to fill the table in a readable way:
//
//   TableBuilder<Ctx> b;
//   b.state(StateId::StartWifi).action(&startWifi).then(StateId::WaitingWifi);
//   b.state(StateId::WaitingWifi).action(&waitWifi)
//       .accepts(EventFlag::WifiConnected, EventFlag::Error);
//   constexpr StateTable<Ctx> table = b.table();
template <typename Context>
class TableBuilder
{
    public:
        class RowBuilder
        {
            public:
                constexpr explicit RowBuilder(StateRow<Context> &row) noexcept
                    : row_(row)
                { }

                constexpr RowBuilder & action(typename StateRow<Context>::ActionFn fn) noexcept
                {
                    row_.action = fn;

                    return *this;
                }

                // Makes the state transient: after its action it always
                // moves to `next`.
                constexpr RowBuilder & then(StateId next) noexcept
                {
                    row_.then = Transition::go(next);

                    return *this;
                }

                template <typename... Flags>
                constexpr RowBuilder & accepts(Flags... flags) noexcept
                {
                    row_.accepts |= maskOf(flags...);

                    return *this;
                }

                constexpr RowBuilder & acceptsMask(std::uint32_t mask) noexcept
                {
                    row_.accepts |= mask;

                    return *this;
                }

            private:
                StateRow<Context> &row_;
        };

        [[nodiscard]]
        constexpr RowBuilder state(StateId id) noexcept
        {
            return RowBuilder(table_[toIndex(id)]);
        }

        [[nodiscard]]
        constexpr StateTable<Context> const & table() const noexcept
        {
            return table_;
        }

    private:
        StateTable<Context> table_{};
};

}
