#pragma once

#include "session/StateBase.hpp"


namespace aa::session
{

class ConnectingState final : public StateBase
{
public:
    [[nodiscard]]
    aa::core::StateId id() const noexcept override
    {
        return aa::core::StateId::Connecting;
    }

    [[nodiscard]]
    std::string_view name() const noexcept override
    {
        return "Connecting";
    }

    void onEnter(aa::core::StateMachine &) override;

    aa::core::Transition handleEvent(aa::core::StateMachine &,
                                        aa::core::Event const &) override;
};

}