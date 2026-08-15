
#include "session/states/WaitingServicesState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;
using aa::session::WaitingServicesState;
using aa::test::MockStateContext;

TEST(WaitingServicesStateTest, ServicesReadyEventTransitionsToRunningState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingServicesState waitingServices;

    machine.setInitialState(waitingServices);
    machine.dispatch(ServicesReadyEvent{});

    EXPECT_EQ(machine.currentState(), &context.running);
}

TEST(WaitingServicesStateTest, BleDisconnectedEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingServicesState waitingServices;

    machine.setInitialState(waitingServices);
    machine.dispatch(BleDisconnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}

TEST(WaitingServicesStateTest, ErrorEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingServicesState waitingServices;

    machine.setInitialState(waitingServices);
    machine.dispatch(ErrorEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}
