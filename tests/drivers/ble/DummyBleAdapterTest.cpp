#include "drivers/ble/DummyBleAdapter.hpp"

#include <gtest/gtest.h>

#include "core/events/BleConnectedEvent.hpp"
#include "core/events/BleDisconnectedEvent.hpp"
#include "core/events/Event.hpp"
#include "core/events/EventQueue.hpp"

namespace
{

class DummyBleAdapterTest : public ::testing::Test
{
protected:
    aa::core::EventQueue queue;
    aa::drivers::ble::DummyBleAdapter adapter{queue};
};

} // namespace

TEST_F(DummyBleAdapterTest, InitializeMarksAdapterInitialized)
{
    EXPECT_FALSE(adapter.initialized());

    ASSERT_TRUE(adapter.initialize());

    EXPECT_TRUE(adapter.initialized());
}

TEST_F(DummyBleAdapterTest, AdvertisingRequiresInitialization)
{
    EXPECT_FALSE(adapter.startAdvertising());
    EXPECT_FALSE(adapter.advertising());
}

TEST_F(DummyBleAdapterTest, StartAdvertisingAfterInitialization)
{
    ASSERT_TRUE(adapter.initialize());

    ASSERT_TRUE(adapter.startAdvertising());

    EXPECT_TRUE(adapter.advertising());
}

TEST_F(DummyBleAdapterTest, StopAdvertisingStopsAdvertising)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.stopAdvertising();

    EXPECT_FALSE(adapter.advertising());
}

TEST_F(DummyBleAdapterTest, ConnectionProducesBleConnectedEvent)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();

    ASSERT_FALSE(queue.empty());

    const aa::core::Event event = queue.pop();

    ASSERT_TRUE(std::holds_alternative<aa::core::BleConnectedEvent>(event));
    EXPECT_TRUE(adapter.connected());
    EXPECT_FALSE(adapter.advertising());
}

TEST_F(DummyBleAdapterTest, DisconnectionProducesBleDisconnectedEvent)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();
    ASSERT_FALSE(queue.empty());
    (void) queue.pop();

    adapter.simulateDisconnection();

    ASSERT_FALSE(queue.empty());

    const aa::core::Event event = queue.pop();

    ASSERT_TRUE(std::holds_alternative<aa::core::BleDisconnectedEvent>(event));
    EXPECT_FALSE(adapter.connected());
}

TEST_F(DummyBleAdapterTest, ConnectionBeforeAdvertisingIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());

    adapter.simulateConnection();

    EXPECT_FALSE(adapter.connected());
    EXPECT_TRUE(queue.empty());
}

TEST_F(DummyBleAdapterTest, DuplicateConnectionIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();
    adapter.simulateConnection();

    ASSERT_FALSE(queue.empty());

    (void) queue.pop();

    EXPECT_TRUE(queue.empty());
}

TEST_F(DummyBleAdapterTest, DuplicateDisconnectionIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();

    ASSERT_FALSE(queue.empty());
    (void) queue.pop();

    adapter.simulateDisconnection();
    adapter.simulateDisconnection();

    ASSERT_FALSE(queue.empty());

    (void) queue.pop();

    EXPECT_TRUE(queue.empty());
}

TEST_F(DummyBleAdapterTest, StopAdvertisingDoesNotDisconnect)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.stopAdvertising();

    EXPECT_FALSE(adapter.connected());
    EXPECT_TRUE(queue.empty());
}

TEST_F(DummyBleAdapterTest, PollDoesNotGenerateEvents)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.poll();

    EXPECT_TRUE(queue.empty());
}
