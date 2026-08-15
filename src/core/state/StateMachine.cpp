
#include "core/state/StateMachine.hpp"

#include <variant>

namespace aa::core
{

StateMachine::StateMachine(IStateContext& context) noexcept : context_(context)
{}

void StateMachine::setInitialState(State& initial)
{
    currentState_ = nullptr;

    transitionTo(initial);
}

void StateMachine::transitionTo(State& next)
{
    if (currentState_ != nullptr)
    {
        currentState_->onExit(*this);
    }

    currentState_ = &next;

    currentState_->onEnter(*this);
}

void StateMachine::dispatch(const Event& event)
{
    if (currentState_ == nullptr)
    {
        return;
    }

    std::visit([this](const auto& alternative) { currentState_->onEvent(*this, alternative); },
               event);
}

State* StateMachine::currentState() const noexcept
{
    return currentState_;
}

IStateContext& StateMachine::context() noexcept
{
    return context_;
}

} // namespace aa::core
