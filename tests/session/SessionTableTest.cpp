
#include "session/SessionContext.hpp"

#include <gtest/gtest.h>

#include <array>
#include <string>
#include <vector>

#include "core/state/EncodeFsm.hpp"
#include "RecordingServices.hpp"

using namespace aa::core;
using aa::session::SessionContext;
using aa::session::sessionTable;
using aa::test::RecordingServices;

namespace
{

struct Fixture
{
    RecordingServices services;
    SessionContext context{services};
    EventFlags events;

    StateId step(StateId state) { return encodeFSM(sessionTable(), state, context, events); }
};

constexpr std::array<EventFlag, 9> AllFlags{
    EventFlag::BleConnected,    EventFlag::BleDisconnected, EventFlag::WifiConnected,
    EventFlag::WifiDisconnected, EventFlag::TransportReady,  EventFlag::PairingRequest,
    EventFlag::PairingCompleted, EventFlag::ServicesReady,   EventFlag::Error,
};

struct Edge
{
    StateId from;
    EventFlag flag;
    StateId to;
};

// Every transition a single tick can take out of a state that waits.
// Anything not listed must leave the state alone.
std::vector<Edge> expectedEdges()
{
    std::vector<Edge> edges{
        {StateId::Disconnected, EventFlag::BleConnected, StateId::StartWifi},
        {StateId::WaitingWifi, EventFlag::WifiConnected, StateId::StartTransport},
        {StateId::WaitingTransport, EventFlag::TransportReady, StateId::StartProtocol},
        {StateId::WaitingProtocol, EventFlag::PairingRequest, StateId::StartAuthentication},
        {StateId::WaitingAuthentication, EventFlag::PairingCompleted, StateId::StartServices},
        {StateId::WaitingServices, EventFlag::ServicesReady, StateId::Running},
    };

    // WaitingWifi ignores WifiDisconnected (wifi is not up yet); every
    // later state treats it as a fault, like BleDisconnected and Error.
    for (StateId from : {StateId::WaitingWifi, StateId::WaitingTransport, StateId::WaitingProtocol,
                         StateId::WaitingAuthentication, StateId::WaitingServices, StateId::Running})
    {
        edges.push_back({from, EventFlag::BleDisconnected, StateId::Recovery});
        edges.push_back({from, EventFlag::Error, StateId::Recovery});

        if (from != StateId::WaitingWifi)
        {
            edges.push_back({from, EventFlag::WifiDisconnected, StateId::Recovery});
        }
    }

    return edges;
}

constexpr std::array<StateId, 7> WaitingStates{
    StateId::Disconnected,        StateId::WaitingWifi,    StateId::WaitingTransport,
    StateId::WaitingProtocol,     StateId::WaitingAuthentication, StateId::WaitingServices,
    StateId::Running,
};

} // namespace

TEST(SessionTableTest, EveryListedEdgeIsTakenAndEveryOtherEventIsDroppedWithoutEffect)
{
    auto const edges = expectedEdges();

    for (StateId const state : WaitingStates)
    {
        for (EventFlag const flag : AllFlags)
        {
            StateId expected = state;
            for (Edge const& edge : edges)
            {
                if (edge.from == state && edge.flag == flag)
                {
                    expected = edge.to;
                }
            }

            Fixture f;
            f.events.set(flag);

            EXPECT_EQ(f.step(state), expected)
                << "from state " << static_cast<int>(state) << " with flag " << bit(flag);

            // Consumed if it made sense, dropped if not: never left behind.
            EXPECT_EQ(f.events.pending(), 0u)
                << "from state " << static_cast<int>(state) << " with flag " << bit(flag);

            // And a waiting state never touches the drivers by itself.
            EXPECT_TRUE(f.services.calls.empty());
        }
    }
}

TEST(SessionTableTest, WithNoEventAWaitingStateJustStaysTickAfterTick)
{
    for (StateId const state : WaitingStates)
    {
        Fixture f;

        for (int i = 0; i < 5; ++i)
        {
            ASSERT_EQ(f.step(state), state);
        }
    }
}

TEST(SessionTableTest, FaultWinsOverAdvanceWhenBothArePending)
{
    Fixture f;
    f.events.set(EventFlag::TransportReady);
    f.events.set(EventFlag::Error);

    EXPECT_EQ(f.step(StateId::WaitingTransport), StateId::Recovery);
}

TEST(SessionTableTest, TransientStatesAreExactlyStartAndRecovery)
{
    auto const& table = sessionTable();

    auto const next = [&table](StateId id) { return table[toIndex(id)].then; };

    EXPECT_EQ(next(StateId::StartWifi).target(), StateId::WaitingWifi);
    EXPECT_EQ(next(StateId::StartTransport).target(), StateId::WaitingTransport);
    EXPECT_EQ(next(StateId::StartProtocol).target(), StateId::WaitingProtocol);
    EXPECT_EQ(next(StateId::StartAuthentication).target(), StateId::WaitingAuthentication);
    EXPECT_EQ(next(StateId::StartServices).target(), StateId::WaitingServices);
    EXPECT_EQ(next(StateId::Recovery).target(), StateId::Disconnected);

    for (StateId const state : WaitingStates)
    {
        EXPECT_FALSE(next(state).valid()) << "state " << static_cast<int>(state);
    }
}

TEST(SessionTableTest, TransientStatesAcceptNoEventsAndEveryStateHasAnAction)
{
    auto const& table = sessionTable();

    for (std::size_t i = 0; i < StateCount; ++i)
    {
        EXPECT_NE(table[i].action, nullptr) << "state " << i;

        if (table[i].then.valid())
        {
            EXPECT_EQ(table[i].accepts, 0u) << "transient state " << i;
        }
    }
}

TEST(SessionTableTest, StartWifiStartsAndConnectsWifiThenMovesToWaitingWifi)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::StartWifi), StateId::WaitingWifi);
    EXPECT_EQ(f.services.calls, (std::vector<std::string>{"wifi.start", "wifi.connect"}));
}

TEST(SessionTableTest, StartTransportOpensTransport)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::StartTransport), StateId::WaitingTransport);
    EXPECT_EQ(f.services.calls, std::vector<std::string>{"transport.open"});
}

TEST(SessionTableTest, StartProtocolStartsProtocol)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::StartProtocol), StateId::WaitingProtocol);
    EXPECT_EQ(f.services.calls, std::vector<std::string>{"protocol.start"});
}

TEST(SessionTableTest, StartAuthenticationStartsAndPairsBluetooth)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::StartAuthentication), StateId::WaitingAuthentication);
    EXPECT_EQ(f.services.calls, (std::vector<std::string>{"bluetooth.start", "bluetooth.pair"}));
}

TEST(SessionTableTest, RecoveryTearsDownHighestLayerFirstThenReturnsToDisconnected)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::Recovery), StateId::Disconnected);
    EXPECT_EQ(f.services.calls,
              (std::vector<std::string>{"bluetooth.unpair", "bluetooth.stop", "protocol.stop",
                                        "transport.close", "wifi.disconnect", "wifi.stop"}));
}
