#pragma once

#include "session/StateBase.hpp"


namespace aa::session
{

class IdleState final : public StateBase
{
public:
    [[nodiscard]]
    aa::core::StateId id() const noexcept override
    {
        return aa::core::StateId::Idle;
    }

    [[nodiscard]]
    std::string_view name() const noexcept override
    {
        return "Idle";
    }

    void onEnter(aa::core::StateMachine &) override;

    aa::core::Transition handleEvent(aa::core::StateMachine &,
                                        aa::core::Event const &) override;
};

}