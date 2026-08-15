
#include "session/states/RunningState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;
using aa::session::RunningState;
using aa::test::MockStateContext;

TEST(RunningStateTest, BleDisconnectedEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    RunningState running;

    machine.setInitialState(running);
    machine.dispatch(BleDisconnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}

TEST(RunningStateTest, WifiDisconnectedEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    RunningState running;

    machine.setInitialState(running);
    machine.dispatch(WifiDisconnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}

TEST(RunningStateTest, ErrorEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    RunningState running;

    machine.setInitialState(running);
    machine.dispatch(ErrorEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}
