
#include "drivers/linux/LinuxBluetooth.hpp"

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include <memory>

#include "drivers/linux/IBluezBackend.hpp"

namespace
{

using ::testing::StrictMock;

class MockBluezBackend : public aa::drivers::linux::IBluezBackend
{
public:
    MOCK_METHOD(bool, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(bool, pair, (), (override));
    MOCK_METHOD(void, unpair, (), (override));
    MOCK_METHOD(bool, paired, (), (const, override));
};

class LinuxBluetoothTest : public ::testing::Test
{
protected:
    StrictMock<MockBluezBackend>* backend = nullptr;

    std::unique_ptr<aa::drivers::linux::LinuxBluetooth> makeBluetooth()
    {
        auto mock = std::make_unique<StrictMock<MockBluezBackend>>();

        backend = mock.get();

        return std::make_unique<aa::drivers::linux::LinuxBluetooth>(std::move(mock));
    }
};

TEST_F(LinuxBluetoothTest, StartsBackend)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    bluetooth->start();

    EXPECT_TRUE(bluetooth->started());
}

TEST_F(LinuxBluetoothTest, FailedStartLeavesBluetoothStopped)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(false));

    bluetooth->start();

    EXPECT_FALSE(bluetooth->started());
}

TEST_F(LinuxBluetoothTest, StartingTwiceOnlyStartsBackendOnce)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    bluetooth->start();
    bluetooth->start();

    EXPECT_TRUE(bluetooth->started());
}

TEST_F(LinuxBluetoothTest, PairRequiresStartedBluetooth)
{
    auto bluetooth = makeBluetooth();

    bluetooth->pair();

    EXPECT_FALSE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, SuccessfulPairMarksBluetoothPaired)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, pair()).WillOnce(::testing::Return(true));

    bluetooth->start();
    bluetooth->pair();

    EXPECT_TRUE(bluetooth->started());
    EXPECT_TRUE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, FailedPairLeavesBluetoothUnpaired)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, pair()).WillOnce(::testing::Return(false));

    bluetooth->start();
    bluetooth->pair();

    EXPECT_TRUE(bluetooth->started());
    EXPECT_FALSE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, StopStopsBackendAndClearsState)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, pair()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, stop());

    bluetooth->start();
    bluetooth->pair();
    bluetooth->stop();

    EXPECT_FALSE(bluetooth->started());
    EXPECT_FALSE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, UnpairClearsPairedState)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, start()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, pair()).WillOnce(::testing::Return(true));

    EXPECT_CALL(*backend, unpair());

    bluetooth->start();
    bluetooth->pair();
    bluetooth->unpair();

    EXPECT_FALSE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, UnpairIsSafeWhenNotPaired)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, unpair());

    bluetooth->unpair();

    EXPECT_FALSE(bluetooth->paired());
}

TEST_F(LinuxBluetoothTest, StopCanBeCalledWithoutStarting)
{
    auto bluetooth = makeBluetooth();

    EXPECT_CALL(*backend, stop());

    bluetooth->stop();

    EXPECT_FALSE(bluetooth->started());
    EXPECT_FALSE(bluetooth->paired());
}

} // namespace