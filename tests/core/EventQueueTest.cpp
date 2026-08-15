
#include "core/events/EventQueue.hpp"

#include <gtest/gtest.h>

using namespace aa::core;

TEST(EventQueueTest, IsEmptyByDefault)
{
    EventQueue queue;

    EXPECT_TRUE(queue.empty());
}

TEST(EventQueueTest, PushMakesItNonEmpty)
{
    EventQueue queue;

    queue.push(BleConnectedEvent{});

    EXPECT_FALSE(queue.empty());
}

TEST(EventQueueTest, PopReturnsPushedEvent)
{
    EventQueue queue;

    queue.push(BleConnectedEvent{});

    Event event = queue.pop();

    EXPECT_TRUE(std::holds_alternative<BleConnectedEvent>(event));
}

TEST(EventQueueTest, PopFollowsFifoOrder)
{
    EventQueue queue;

    queue.push(BleConnectedEvent{});
    queue.push(BleDisconnectedEvent{});

    Event first = queue.pop();
    Event second = queue.pop();

    EXPECT_TRUE(std::holds_alternative<BleConnectedEvent>(first));
    EXPECT_TRUE(std::holds_alternative<BleDisconnectedEvent>(second));
}

TEST(EventQueueTest, PopDrainsTheQueue)
{
    EventQueue queue;

    queue.push(BleConnectedEvent{});
    [[maybe_unused]] Event popped = queue.pop();

    EXPECT_TRUE(queue.empty());
}
