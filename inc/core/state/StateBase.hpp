#pragma once

#include "core/state/State.hpp"
#include "core/state/Transition.hpp"


namespace aa::session
{

class StateBase : public aa::core::State
{
    protected:
        [[nodiscard]]
        static aa::core::Transition stay() noexcept
        {
            return aa::core::Transition::stay();
        }

        [[nodiscard]]
        static aa::core::Transition go(aa::core::StateId id) noexcept
        {
            return aa::core::Transition::go(id);
        }
};

}