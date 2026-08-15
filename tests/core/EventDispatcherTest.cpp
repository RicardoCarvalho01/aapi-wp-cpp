
#include "core/events/EventDispatcher.hpp"

#include <gtest/gtest.h>

#include "core/events/EventQueue.hpp"
#include "core/state/IStateContext.hpp"
#include "core/state/State.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;

namespace
{

class SpyState : public State
{
public:
    int bleConnectedCount{0};
    int wifiConnectedCount{0};

    void onEvent(StateMachine&, const BleConnectedEvent&) override { bleConnectedCount++; }

    void onEvent(StateMachine&, const WifiConnectedEvent&) override { wifiConnectedCount++; }
};

class SingleStateContext final : public IStateContext
{
public:
    SpyState state;

    State& disconnectedState() override { return state; }

    State& startWifiState() override { return state; }

    State& waitingWifiState() override { return state; }

    State& startTransportState() override { return state; }

    State& waitingTransportState() override { return state; }

    State& startProtocolState() override { return state; }

    State& waitingProtocolState() override { return state; }

    State& startAuthenticationState() override { return state; }

    State& waitingAuthenticationState() override { return state; }

    State& startServicesState() override { return state; }

    State& waitingServicesState() override { return state; }

    State& runningState() override { return state; }

    State& recoveryState() override { return state; }
};

} // namespace

TEST(EventDispatcherTest, DispatchProcessesOneQueuedEvent)
{
    SingleStateContext context;
    StateMachine machine{context};
    machine.setInitialState(context.state);

    EventQueue queue;
    queue.push(BleConnectedEvent{});
    queue.push(WifiConnectedEvent{});

    EventDispatcher dispatcher{queue, machine};
    dispatcher.dispatch();

    EXPECT_EQ(context.state.bleConnectedCount, 1);
    EXPECT_EQ(context.state.wifiConnectedCount, 0);
    EXPECT_EQ(queue.size(), 1u);
}

TEST(EventDispatcherTest, DispatchAllDrainsTheWholeQueue)
{
    SingleStateContext context;
    StateMachine machine{context};
    machine.setInitialState(context.state);

    EventQueue queue;
    queue.push(BleConnectedEvent{});
    queue.push(WifiConnectedEvent{});
    queue.push(BleConnectedEvent{});

    EventDispatcher dispatcher{queue, machine};
    dispatcher.dispatchAll();

    EXPECT_EQ(context.state.bleConnectedCount, 2);
    EXPECT_EQ(context.state.wifiConnectedCount, 1);
    EXPECT_TRUE(queue.empty());
}

TEST(EventDispatcherTest, DispatchOnEmptyQueueIsANoOp)
{
    SingleStateContext context;
    StateMachine machine{context};
    machine.setInitialState(context.state);

    EventQueue queue;
    EventDispatcher dispatcher{queue, machine};

    dispatcher.dispatch();
    dispatcher.dispatchAll();

    EXPECT_EQ(context.state.bleConnectedCount, 0);
}
