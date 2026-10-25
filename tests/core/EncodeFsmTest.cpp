
#include "core/state/EncodeFsm.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace aa::core;

namespace
{

struct Ctx
{
    std::vector<int> ran; // which actions ran, in order
};

// Labels: A = Disconnected (waits for BleConnected), T = StartWifi
// (transient -> WaitingWifi), W = WaitingWifi (waits for WifiConnected).

Transition actionA(Ctx& ctx, EventFlags& events)
{
    ctx.ran.push_back('A');
    return events.consume(EventFlag::BleConnected) ? Transition::go(StateId::StartWifi)
                                                   : Transition::stay();
}

// Asks to stay, but is transient: `then` must win.
Transition actionT(Ctx& ctx, EventFlags&)
{
    ctx.ran.push_back('T');
    return Transition::stay();
}

// Asks to go somewhere, but is transient: `then` must win.
Transition actionTWithOpinion(Ctx& ctx, EventFlags&)
{
    ctx.ran.push_back('t');
    return Transition::go(StateId::Running);
}

Transition actionW(Ctx& ctx, EventFlags& events)
{
    ctx.ran.push_back('W');
    return events.consume(EventFlag::WifiConnected) ? Transition::go(StateId::Running)
                                                    : Transition::stay();
}

StateTable<Ctx> makeTable()
{
    TableBuilder<Ctx> b;

    b.state(StateId::Disconnected).action(&actionA).accepts(EventFlag::BleConnected);
    b.state(StateId::StartWifi).action(&actionT).then(StateId::WaitingWifi);
    b.state(StateId::WaitingWifi).action(&actionW).accepts(EventFlag::WifiConnected);
    b.state(StateId::StartTransport).action(&actionTWithOpinion).then(StateId::WaitingWifi);
    // Running: no action, no `then` -> inert.

    return b.table();
}

struct Fixture
{
    StateTable<Ctx> table{makeTable()};
    Ctx ctx;
    EventFlags events;

    StateId step(StateId s) { return encodeFSM(table, s, ctx, events); }
};

} // namespace

TEST(EncodeFsmTest, WaitingStateRunsItsActionEveryTickAndStaysWithoutInput)
{
    Fixture f;
    StateId s = StateId::Disconnected;

    s = f.step(s);
    s = f.step(s);
    s = f.step(s);

    EXPECT_EQ(s, StateId::Disconnected);
    EXPECT_EQ(f.ctx.ran, (std::vector<int>{'A', 'A', 'A'}));
}

TEST(EncodeFsmTest, ActionConsumingAFlagReturnsTheNextState)
{
    Fixture f;
    f.events.set(EventFlag::BleConnected);

    EXPECT_EQ(f.step(StateId::Disconnected), StateId::StartWifi);
    EXPECT_EQ(f.events.pending(), 0u);
}

TEST(EncodeFsmTest, TransientStateRunsOnceThenMovesOnWithNoInput)
{
    Fixture f;

    StateId s = f.step(StateId::StartWifi);

    EXPECT_EQ(s, StateId::WaitingWifi);
    EXPECT_EQ(f.ctx.ran, std::vector<int>{'T'});
}

TEST(EncodeFsmTest, AutomaticSuccessorBeatsWhateverTheActionReturned)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::StartTransport), StateId::WaitingWifi);
}

TEST(EncodeFsmTest, EventsThatMakeNoSenseInTheCurrentStateAreDropped)
{
    Fixture f;
    f.events.set(EventFlag::WifiConnected); // Disconnected does not accept it
    f.events.set(EventFlag::BleConnected);

    (void) f.step(StateId::Disconnected);

    EXPECT_EQ(f.events.pending(), 0u); // BleConnected consumed, Wifi dropped
}

TEST(EncodeFsmTest, DroppedEventDoesNotResurfaceInALaterState)
{
    Fixture f;
    f.events.set(EventFlag::WifiConnected);

    StateId s = f.step(StateId::Disconnected); // dropped here
    ASSERT_EQ(s, StateId::Disconnected);

    s = f.step(StateId::WaitingWifi); // would have advanced if it had survived

    EXPECT_EQ(s, StateId::WaitingWifi);
}

TEST(EncodeFsmTest, StateWithoutActionOrSuccessorJustStays)
{
    Fixture f;

    EXPECT_EQ(f.step(StateId::Running), StateId::Running);
    EXPECT_TRUE(f.ctx.ran.empty());
}
