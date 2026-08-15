
#include "session/states/StartWifiState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using aa::session::StartWifiState;
using aa::test::MockStateContext;

TEST(StartWifiStateTest, OnEnterImmediatelyTransitionsToWaitingWifiState)
{
    MockStateContext context;
    aa::core::StateMachine machine{context};
    StartWifiState startWifi;

    // StartWifiState is transient by design: as soon as it becomes the
    // current state, it kicks off the wifi connection (TODO) and moves
    // straight into WaitingWifiState.
    machine.setInitialState(startWifi);

    EXPECT_EQ(machine.currentState(), &context.waitingWifi);
}
