
#include "session/states/WaitingProtocolState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;
using aa::session::WaitingProtocolState;
using aa::test::MockStateContext;

TEST(WaitingProtocolStateTest, PairingRequestEventTransitionsToStartAuthenticationState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingProtocolState waitingProtocol;

    machine.setInitialState(waitingProtocol);
    machine.dispatch(PairingRequestEvent{});

    EXPECT_EQ(machine.currentState(), &context.startAuthentication);
}

TEST(WaitingProtocolStateTest, BleDisconnectedEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingProtocolState waitingProtocol;

    machine.setInitialState(waitingProtocol);
    machine.dispatch(BleDisconnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}

TEST(WaitingProtocolStateTest, WifiDisconnectedEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingProtocolState waitingProtocol;

    machine.setInitialState(waitingProtocol);
    machine.dispatch(WifiDisconnectedEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}

TEST(WaitingProtocolStateTest, ErrorEventTransitionsToRecoveryState)
{
    MockStateContext context;
    StateMachine machine{context};
    WaitingProtocolState waitingProtocol;

    machine.setInitialState(waitingProtocol);
    machine.dispatch(ErrorEvent{});

    EXPECT_EQ(machine.currentState(), &context.recovery);
}
