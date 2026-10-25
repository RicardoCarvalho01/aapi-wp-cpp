
#include "core/events/EventDispatcher.hpp"

#include <gtest/gtest.h>

#include "core/events/EventQueue.hpp"
#include "core/state/StateMachine.hpp"

using namespace aa::core;

namespace
{

struct Counters
{
    int ble{0};
    int wifi{0};
};

Transition countBle(Counters& c, const Event&)
{
    c.ble++;
    return Transition::stay();
}

Transition countWifi(Counters& c, const Event&)
{
    c.wifi++;
    return Transition::stay();
}

StateTable<Counters> makeTable()
{
    StateTable<Counters> table{};

    table[toIndex(StateId::Disconnected)].on[EventIndex<BleConnectedEvent>] = &countBle;
    table[toIndex(StateId::Disconnected)].on[EventIndex<WifiConnectedEvent>] = &countWifi;

    return table;
}

struct Fixture
{
    Counters counters;
    StateTable<Counters> table{makeTable()};
    StateMachine<Counters> machine{counters, table};
    EventQueue queue;
    EventDispatcher<StateMachine<Counters>> dispatcher{queue, machine};

    Fixture() { machine.start(StateId::Disconnected); }
};

} // namespace

TEST(EventDispatcherTest, DispatchProcessesOneQueuedEvent)
{
    Fixture f;
    f.queue.push(BleConnectedEvent{});
    f.queue.push(WifiConnectedEvent{});

    f.dispatcher.dispatch();

    EXPECT_EQ(f.counters.ble, 1);
    EXPECT_EQ(f.counters.wifi, 0);
    EXPECT_EQ(f.queue.size(), 1u);
}

TEST(EventDispatcherTest, DispatchAllDrainsTheWholeQueue)
{
    Fixture f;
    f.queue.push(BleConnectedEvent{});
    f.queue.push(WifiConnectedEvent{});
    f.queue.push(BleConnectedEvent{});

    f.dispatcher.dispatchAll();

    EXPECT_EQ(f.counters.ble, 2);
    EXPECT_EQ(f.counters.wifi, 1);
    EXPECT_TRUE(f.queue.empty());
}

TEST(EventDispatcherTest, DispatchOnEmptyQueueIsANoOp)
{
    Fixture f;

    f.dispatcher.dispatch();
    f.dispatcher.dispatchAll();

    EXPECT_EQ(f.counters.ble, 0);
    EXPECT_EQ(f.counters.wifi, 0);
}
