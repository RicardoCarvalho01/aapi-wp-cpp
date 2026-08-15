
#include "session/Session.hpp"

#include <gtest/gtest.h>

using namespace aa::core;
using aa::session::Session;

TEST(SessionTest, StartsInDisconnectedState)
{
    Session session;

    EXPECT_EQ(session.currentState(), &session.disconnectedState());
}

TEST(SessionTest, BleConnectedEventEndsUpInWaitingWifiState)
{
    Session session;

    session.dispatch(BleConnectedEvent{});

    // DisconnectedState transitions to StartWifiState, but StartWifiState
    // is transient: its onEnter() kicks off the wifi connection and
    // immediately self-transitions to WaitingWifiState.
    EXPECT_EQ(session.currentState(), &session.waitingWifiState());
}

TEST(SessionTest, UnhandledEventInDisconnectedStateIsIgnored)
{
    Session session;

    session.dispatch(WifiConnectedEvent{});

    EXPECT_EQ(session.currentState(), &session.disconnectedState());
}

TEST(SessionTest, FullHandshakeEndsUpInRunningState)
{
    Session session;

    // Walks the entire happy path: Ble -> Wifi -> Transport -> Protocol
    // -> Authentication -> Services -> Running. Each Start*State is
    // transient and self-transitions to the matching Waiting*State,
    // which is why only one dispatch() per real-world signal is needed.
    session.dispatch(BleConnectedEvent{});
    session.dispatch(WifiConnectedEvent{});
    session.dispatch(TransportReadyEvent{});
    session.dispatch(PairingRequestEvent{});
    session.dispatch(PairingCompletedEvent{});
    session.dispatch(ServicesReadyEvent{});

    EXPECT_EQ(session.currentState(), &session.runningState());
}

TEST(SessionTest, DisconnectMidHandshakeRecoversBackToDisconnectedState)
{
    Session session;

    session.dispatch(BleConnectedEvent{});
    session.dispatch(WifiConnectedEvent{});
    session.dispatch(TransportReadyEvent{});

    // A BLE drop partway through the handshake should send the session
    // through RecoveryState and back to square one, not leave it stuck.
    session.dispatch(BleDisconnectedEvent{});

    EXPECT_EQ(session.currentState(), &session.disconnectedState());
}

TEST(SessionTest, ErrorEventWhileRunningRecoversBackToDisconnectedState)
{
    Session session;

    session.dispatch(BleConnectedEvent{});
    session.dispatch(WifiConnectedEvent{});
    session.dispatch(TransportReadyEvent{});
    session.dispatch(PairingRequestEvent{});
    session.dispatch(PairingCompletedEvent{});
    session.dispatch(ServicesReadyEvent{});
    ASSERT_EQ(session.currentState(), &session.runningState());

    session.dispatch(ErrorEvent{});

    EXPECT_EQ(session.currentState(), &session.disconnectedState());
}
