
#include "session/states/DisconnectedState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;
using namespace aa::session;
using aa::test::MockStateContext;

TEST(DisconnectedStateTest, BleConnectedEventTransitionsToStartWifiState)
{
    MockStateContext context;
    StateMachine machine{context};
    DisconnectedState disconnected;

    machine.setInitialState(disconnected);
    machine.dispatch(BleConnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.startWifi);
}

TEST(DisconnectedStateTest, OtherEventsAreIgnored)
{
    MockStateContext context;
    StateMachine machine{context};
    DisconnectedState disconnected;

    machine.setInitialState(disconnected);
    machine.dispatch(WifiConnectedEvent{});

    EXPECT_EQ(machine.currentState(), &disconnected);
}
