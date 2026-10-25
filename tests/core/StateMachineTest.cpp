
#include "core/state/StateMachine.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "core/state/StateTable.hpp"

using namespace aa::core;

namespace
{

// The machine is generic over its context; here the context is just a log.
struct Log
{
    std::vector<std::string> entries;
};

using Table = StateTable<Log>;

void logEnterA(Log& log) { log.entries.emplace_back("enter A"); }

Transition enterA(Log& log)
{
    logEnterA(log);
    return Transition::stay();
}

void exitA(Log& log) { log.entries.emplace_back("exit A"); }

Transition enterB(Log& log)
{
    log.entries.emplace_back("enter B");
    return Transition::stay();
}

// Transient: logs and hands off to C immediately.
Transition enterTransient(Log& log)
{
    log.entries.emplace_back("enter T");
    return Transition::go(StateId::Running);
}

Transition enterC(Log& log)
{
    log.entries.emplace_back("enter C");
    return Transition::stay();
}

Transition goToB(Log& log, const Event&)
{
    log.entries.emplace_back("handler goToB");
    return Transition::go(StateId::StartWifi);
}

Transition goToTransient(Log&, const Event&) { return Transition::go(StateId::WaitingWifi); }

Transition stayPut(Log& log, const Event&)
{
    log.entries.emplace_back("handler stay");
    return Transition::stay();
}

// States are just labels here: A = Disconnected, B = StartWifi,
// T = WaitingWifi (transient), C = Running.
Table makeTable()
{
    Table table{};

    table[toIndex(StateId::Disconnected)].onEnter = &enterA;
    table[toIndex(StateId::Disconnected)].onExit = &exitA;
    table[toIndex(StateId::Disconnected)].on[EventIndex<BleConnectedEvent>] = &goToB;
    table[toIndex(StateId::Disconnected)].on[EventIndex<WifiConnectedEvent>] = &goToTransient;
    table[toIndex(StateId::Disconnected)].on[EventIndex<ErrorEvent>] = &stayPut;

    table[toIndex(StateId::StartWifi)].onEnter = &enterB;

    table[toIndex(StateId::WaitingWifi)].onEnter = &enterTransient;

    table[toIndex(StateId::Running)].onEnter = &enterC;

    return table;
}

} // namespace

TEST(StateMachineTest, HasNoCurrentStateBeforeStart)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};

    EXPECT_FALSE(machine.currentState().has_value());
}

TEST(StateMachineTest, StartMakesStateCurrentAndRunsOnEnterButNoOnExit)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};

    machine.start(StateId::Disconnected);

    EXPECT_EQ(machine.currentState(), StateId::Disconnected);
    EXPECT_EQ(log.entries, std::vector<std::string>{"enter A"});
}

TEST(StateMachineTest, TransitionRunsExitOfOldThenEnterOfNew)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};
    machine.start(StateId::Disconnected);
    log.entries.clear();

    machine.dispatch(BleConnectedEvent{});

    EXPECT_EQ(machine.currentState(), StateId::StartWifi);
    EXPECT_EQ(log.entries, (std::vector<std::string>{"handler goToB", "exit A", "enter B"}));
}

TEST(StateMachineTest, TransientStateFollowsTransitionReturnedByOnEnter)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};
    machine.start(StateId::Disconnected);
    log.entries.clear();

    machine.dispatch(WifiConnectedEvent{});

    EXPECT_EQ(machine.currentState(), StateId::Running);
    EXPECT_EQ(log.entries, (std::vector<std::string>{"exit A", "enter T", "enter C"}));
}

TEST(StateMachineTest, HandlerReturningStayDoesNotTouchEnterOrExit)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};
    machine.start(StateId::Disconnected);
    log.entries.clear();

    machine.dispatch(ErrorEvent{});

    EXPECT_EQ(machine.currentState(), StateId::Disconnected);
    EXPECT_EQ(log.entries, std::vector<std::string>{"handler stay"});
}

TEST(StateMachineTest, EventWithNoHandlerInCurrentStateIsIgnored)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};
    machine.start(StateId::Disconnected);
    log.entries.clear();

    machine.dispatch(ServicesReadyEvent{});

    EXPECT_EQ(machine.currentState(), StateId::Disconnected);
    EXPECT_TRUE(log.entries.empty());
}

TEST(StateMachineTest, DispatchBeforeStartIsANoOp)
{
    Log log;
    Table const table = makeTable();
    StateMachine<Log> machine{log, table};

    machine.dispatch(BleConnectedEvent{});

    EXPECT_FALSE(machine.currentState().has_value());
    EXPECT_TRUE(log.entries.empty());
}

TEST(EventIndexTest, MatchesVariantIndexForEveryAlternative)
{
    EXPECT_EQ(Event{BleConnectedEvent{}}.index(), EventIndex<BleConnectedEvent>);
    EXPECT_EQ(Event{WifiConnectedEvent{}}.index(), EventIndex<WifiConnectedEvent>);
    EXPECT_EQ(Event{ErrorEvent{}}.index(), EventIndex<ErrorEvent>);
    EXPECT_EQ(EventIndex<ErrorEvent>, EventCount - 1);
}
