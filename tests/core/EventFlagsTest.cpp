
#include "core/events/EventFlags.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <thread>

using namespace aa::core;

TEST(EventFlagsTest, StartsEmpty)
{
    EventFlags flags;

    EXPECT_EQ(flags.pending(), 0u);
    EXPECT_FALSE(flags.test(EventFlag::Error));
}

TEST(EventFlagsTest, SetThenConsumeClearsOnlyThatFlag)
{
    EventFlags flags;
    flags.set(EventFlag::WifiConnected);
    flags.set(EventFlag::Error);

    EXPECT_TRUE(flags.consume(EventFlag::WifiConnected));

    EXPECT_FALSE(flags.test(EventFlag::WifiConnected));
    EXPECT_TRUE(flags.test(EventFlag::Error));
}

TEST(EventFlagsTest, ConsumeOfAnUnsetFlagReturnsFalse)
{
    EventFlags flags;

    EXPECT_FALSE(flags.consume(EventFlag::BleConnected));
}

TEST(EventFlagsTest, RaisingTwiceCountsOnce)
{
    EventFlags flags;
    flags.set(EventFlag::BleConnected);
    flags.set(EventFlag::BleConnected);

    EXPECT_TRUE(flags.consume(EventFlag::BleConnected));
    EXPECT_FALSE(flags.consume(EventFlag::BleConnected));
}

TEST(EventFlagsTest, ConsumeAnyClearsEveryFlagInTheMask)
{
    EventFlags flags;
    flags.set(EventFlag::BleDisconnected);
    flags.set(EventFlag::Error);
    flags.set(EventFlag::WifiConnected);

    EXPECT_TRUE(flags.consumeAny(maskOf(EventFlag::BleDisconnected, EventFlag::Error)));

    EXPECT_EQ(flags.pending(), bit(EventFlag::WifiConnected));
    EXPECT_FALSE(flags.consumeAny(maskOf(EventFlag::BleDisconnected, EventFlag::Error)));
}

TEST(EventFlagsTest, ClearExceptDropsEverythingNotKeptAndReportsIt)
{
    EventFlags flags;
    flags.set(EventFlag::WifiConnected);
    flags.set(EventFlag::Error);
    flags.set(EventFlag::ServicesReady);

    auto const dropped = flags.clearExcept(maskOf(EventFlag::Error));

    EXPECT_EQ(dropped, maskOf(EventFlag::WifiConnected, EventFlag::ServicesReady));
    EXPECT_EQ(flags.pending(), bit(EventFlag::Error));
}

TEST(EventFlagsTest, FlagsRaisedFromAnotherThreadAreSeen)
{
    EventFlags flags;
    std::atomic<int> payload{0};

    std::thread producer(
        [&]
        {
            payload = 42; // data first...
            flags.set(EventFlag::PairingRequest); // ...then the flag
        });
    producer.join();

    ASSERT_TRUE(flags.consume(EventFlag::PairingRequest));
    EXPECT_EQ(payload, 42);
}
