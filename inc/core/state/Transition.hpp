#pragma once

#include "core/state/StateId.hpp"


namespace aa::core
{

class Transition
{
    public:
        constexpr Transition() noexcept = default;

        constexpr explicit Transition(StateId target) noexcept
            : valid_(true)
            , target_(target)
        { }

        [[nodiscard]]
        constexpr bool valid() const noexcept
        {
            return valid_;
        }

        [[nodiscard]]
        constexpr StateId target() const noexcept
        {
            return target_;
        }

        [[nodiscard]]
        static constexpr Transition stay() noexcept
        {
            return {};
        }

        [[nodiscard]]
        static constexpr Transition go(StateId target) noexcept
        {
            return Transition(target);
        }

    private:
        bool valid_{false};

        StateId target_{StateId::Idle};
};

}