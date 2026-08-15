
#include "core/state/StateMachine.hpp"

#include <gtest/gtest.h>

#include "core/state/IStateContext.hpp"
#include "core/state/State.hpp"

using namespace aa::core;

namespace
{

class SpyState : public State
{
public:
    int enterCount{0};
    int exitCount{0};
    int bleConnectedCount{0};

    void onEnter(StateMachine&) override { enterCount++; }

    void onExit(StateMachine&) override { exitCount++; }

    void onEvent(StateMachine&, const BleConnectedEvent&) override { bleConnectedCount++; }
};

// None of the tests below exercise machine.context(), so this stub only
// needs to satisfy the IStateContext interface, not do anything useful.
class UnusedStateContext : public IStateContext
{
public:
    State& disconnectedState() override { return unreachable(); }

    State& startWifiState() override { return unreachable(); }

    State& waitingWifiState() override { return unreachable(); }

    State& startTransportState() override { return unreachable(); }

    State& waitingTransportState() override { return unreachable(); }

    State& startProtocolState() override { return unreachable(); }

    State& waitingProtocolState() override { return unreachable(); }

    State& startAuthenticationState() override { return unreachable(); }

    State& waitingAuthenticationState() override { return unreachable(); }

    State& startServicesState() override { return unreachable(); }

    State& waitingServicesState() override { return unreachable(); }

    State& runningState() override { return unreachable(); }

    State& recoveryState() override { return unreachable(); }

private:
    State& unreachable()
    {
        ADD_FAILURE() << "IStateContext should not be used in this test";
        static State dummy;
        return dummy;
    }
};

} // namespace

TEST(StateMachineTest, HasNoCurrentStateBeforeInitialization)
{
    UnusedStateContext context;
    StateMachine machine{context};

    EXPECT_EQ(machine.currentState(), nullptr);
}

TEST(StateMachineTest, SetInitialStateCallsOnEnter)
{
    UnusedStateContext context;
    StateMachine machine{context};
    SpyState state;

    machine.setInitialState(state);

    EXPECT_EQ(machine.currentState(), &state);
    EXPECT_EQ(state.enterCount, 1);
}

TEST(StateMachineTest, TransitionToCallsExitOnOldStateAndEnterOnNewState)
{
    UnusedStateContext context;
    StateMachine machine{context};
    SpyState from;
    SpyState to;

    machine.setInitialState(from);
    machine.transitionTo(to);

    EXPECT_EQ(from.exitCount, 1);
    EXPECT_EQ(to.enterCount, 1);
    EXPECT_EQ(machine.currentState(), &to);
}

TEST(StateMachineTest, DispatchRoutesEventToCurrentStateOnEventOverload)
{
    UnusedStateContext context;
    StateMachine machine{context};
    SpyState state;

    machine.setInitialState(state);
    machine.dispatch(BleConnectedEvent{});

    EXPECT_EQ(state.bleConnectedCount, 1);
}

TEST(StateMachineTest, DispatchBeforeInitialStateIsANoOp)
{
    UnusedStateContext context;
    StateMachine machine{context};

    // must not crash even though no state has been set yet
    machine.dispatch(BleConnectedEvent{});

    EXPECT_EQ(machine.currentState(), nullptr);
}
