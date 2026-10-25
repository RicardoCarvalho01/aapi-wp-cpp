
#include "session/Session.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <chrono>
#include <thread>

#include "RecordingServices.hpp"

using namespace aa::core;
using aa::session::Session;
using aa::test::RecordingServices;

namespace
{

void ticks(Session& session, int n)
{
    for (int i = 0; i < n; ++i)
    {
        session.encodeFSM();
    }
}

// Raises an event and gives the FSM enough ticks to consume it, run the
// transient Start* state it leads to, and settle in the next waiting state.
void raise(Session& session, EventFlag flag)
{
    session.events().set(flag);
    ticks(session, 3);
}

void runHandshake(Session& session)
{
    raise(session, EventFlag::BleConnected);
    raise(session, EventFlag::WifiConnected);
    raise(session, EventFlag::TransportReady);
    raise(session, EventFlag::PairingRequest);
    raise(session, EventFlag::PairingCompleted);
    raise(session, EventFlag::ServicesReady);
}

} // namespace

TEST(SessionTest, StartsInDisconnectedStateAndTouchesNothing)
{
    RecordingServices services;
    Session session(services);

    ticks(session, 10);

    EXPECT_EQ(session.state(), StateId::Disconnected);
    EXPECT_TRUE(services.calls.empty());
}

TEST(SessionTest, ExposesTheInjectedServices)
{
    RecordingServices services;
    Session session(services);

    EXPECT_EQ(&session.services(), &services);
}

TEST(SessionTest, IrrelevantEventInDisconnectedStateIsDropped)
{
    RecordingServices services;
    Session session(services);

    session.events().set(EventFlag::WifiConnected);
    ticks(session, 1);

    EXPECT_EQ(session.state(), StateId::Disconnected);
    EXPECT_EQ(session.events().pending(), 0u);

    // ...and it must not fire later, once the wifi is actually awaited.
    raise(session, EventFlag::BleConnected);
    ASSERT_EQ(session.state(), StateId::WaitingWifi);
    ticks(session, 5);
    EXPECT_EQ(session.state(), StateId::WaitingWifi);
}

TEST(SessionTest, EachStepMovesOneStateAtATime)
{
    RecordingServices services;
    Session session(services);

    session.events().set(EventFlag::BleConnected);

    session.encodeFSM();
    EXPECT_EQ(session.state(), StateId::StartWifi);

    session.encodeFSM();
    EXPECT_EQ(session.state(), StateId::WaitingWifi);
    EXPECT_EQ(services.calls, (std::vector<std::string>{"wifi.start", "wifi.connect"}));
}

TEST(SessionTest, FullHandshakeEndsUpInRunningStateAndDrivesServicesInOrder)
{
    RecordingServices services;
    Session session(services);

    runHandshake(session);

    EXPECT_EQ(session.state(), StateId::Running);
    EXPECT_EQ(services.calls,
              (std::vector<std::string>{"wifi.start", "wifi.connect", "transport.open",
                                        "protocol.start", "bluetooth.start", "bluetooth.pair"}));
}

TEST(SessionTest, DisconnectMidHandshakeRecoversBackToDisconnected)
{
    RecordingServices services;
    Session session(services);

    raise(session, EventFlag::BleConnected);
    raise(session, EventFlag::WifiConnected);
    raise(session, EventFlag::TransportReady);
    services.calls.clear();

    raise(session, EventFlag::BleDisconnected);

    EXPECT_EQ(session.state(), StateId::Disconnected);
    EXPECT_EQ(services.calls.front(), "bluetooth.unpair");
    EXPECT_EQ(services.calls.back(), "wifi.stop");
}

TEST(SessionTest, ErrorWhileRunningRecoversAndCanHandshakeAgain)
{
    RecordingServices services;
    Session session(services);

    runHandshake(session);
    ASSERT_EQ(session.state(), StateId::Running);

    raise(session, EventFlag::Error);
    EXPECT_EQ(session.state(), StateId::Disconnected);

    runHandshake(session);
    EXPECT_EQ(session.state(), StateId::Running);
}

TEST(SessionTest, RunsOnItsOwnThreadAndReactsToEventsRaisedFromAnotherThread)
{
    using namespace std::chrono_literals;

    RecordingServices services;
    Session session(services);
    std::jthread fsm([&session](std::stop_token stop) { session.run(stop, 1ms); });

    auto const waitFor = [&session](StateId target)
    {
        auto const deadline = std::chrono::steady_clock::now() + 2s;
        while (session.state() != target && std::chrono::steady_clock::now() < deadline)
        {
            std::this_thread::sleep_for(1ms);
        }
        return session.state() == target;
    };

    session.events().set(EventFlag::BleConnected);
    ASSERT_TRUE(waitFor(StateId::WaitingWifi));

    session.events().set(EventFlag::WifiConnected);
    ASSERT_TRUE(waitFor(StateId::WaitingTransport));

    fsm.request_stop();
    fsm.join();

    // Stopped for real: no more steps, state frozen.
    auto const frozen = session.state();
    session.events().set(EventFlag::TransportReady);
    std::this_thread::sleep_for(20ms);
    EXPECT_EQ(session.state(), frozen);
}
