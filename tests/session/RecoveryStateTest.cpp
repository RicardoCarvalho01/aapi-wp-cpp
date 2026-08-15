
#include "session/states/RecoveryState.hpp"

#include <gtest/gtest.h>

#include "MockStateContext.hpp"
#include "core/state/StateMachine.hpp"

using aa::session::RecoveryState;
using aa::test::MockStateContext;

TEST(RecoveryStateTest, OnEnterImmediatelyTransitionsToDisconnectedState)
{
    MockStateContext context;
    aa::core::StateMachine machine{context};
    RecoveryState recovery;

    // RecoveryState is transient by design: as soon as it becomes the
    // current state, it tears things down (TODO) and moves straight
    // back to DisconnectedState so the session can start over.
    machine.setInitialState(recovery);

    EXPECT_EQ(machine.currentState(), &context.disconnected);
}
