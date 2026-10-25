#include "drivers/ble/DummyBleAdapter.hpp"

#include <gtest/gtest.h>

#include "core/events/EventFlags.hpp"

using aa::core::EventFlag;

namespace
{

class DummyBleAdapterTest : public ::testing::Test
{
protected:
    aa::core::EventFlags events;
    aa::drivers::ble::DummyBleAdapter adapter{events};
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

    EXPECT_TRUE(events.consume(EventFlag::BleConnected));
    EXPECT_EQ(events.pending(), 0u);
    EXPECT_TRUE(adapter.connected());
    EXPECT_FALSE(adapter.advertising());
}

TEST_F(DummyBleAdapterTest, DisconnectionProducesBleDisconnectedEvent)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();
    ASSERT_TRUE(events.consume(EventFlag::BleConnected));

    adapter.simulateDisconnection();

    EXPECT_TRUE(events.consume(EventFlag::BleDisconnected));
    EXPECT_EQ(events.pending(), 0u);
    EXPECT_FALSE(adapter.connected());
}

TEST_F(DummyBleAdapterTest, ConnectionBeforeAdvertisingIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());

    adapter.simulateConnection();

    EXPECT_FALSE(adapter.connected());
    EXPECT_EQ(events.pending(), 0u);
}

TEST_F(DummyBleAdapterTest, DuplicateConnectionIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();
    ASSERT_TRUE(events.consume(EventFlag::BleConnected));

    adapter.simulateConnection();

    EXPECT_EQ(events.pending(), 0u);
}

TEST_F(DummyBleAdapterTest, DuplicateDisconnectionIsIgnored)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.simulateConnection();
    ASSERT_TRUE(events.consume(EventFlag::BleConnected));

    adapter.simulateDisconnection();
    ASSERT_TRUE(events.consume(EventFlag::BleDisconnected));

    adapter.simulateDisconnection();

    EXPECT_EQ(events.pending(), 0u);
}

TEST_F(DummyBleAdapterTest, StopAdvertisingDoesNotDisconnect)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.stopAdvertising();

    EXPECT_FALSE(adapter.connected());
    EXPECT_EQ(events.pending(), 0u);
}

TEST_F(DummyBleAdapterTest, PollDoesNotGenerateEvents)
{
    ASSERT_TRUE(adapter.initialize());
    ASSERT_TRUE(adapter.startAdvertising());

    adapter.poll();

    EXPECT_EQ(events.pending(), 0u);
}
