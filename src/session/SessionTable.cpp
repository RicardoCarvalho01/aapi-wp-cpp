
#include "session/SessionContext.hpp"

namespace aa::session
{

namespace
{

using aa::core::EventFlag;
using aa::core::EventFlags;
using aa::core::StateId;
using aa::core::TableBuilder;
using aa::core::Transition;

// Faults that send a running handshake to Recovery. WaitingWifi does not
// care about the wifi dropping (it is not up yet), the later states do.
constexpr std::uint32_t FaultsBleOnly =
    aa::core::maskOf(EventFlag::BleDisconnected, EventFlag::Error);

constexpr std::uint32_t FaultsAll =
    aa::core::maskOf(EventFlag::BleDisconnected, EventFlag::WifiDisconnected, EventFlag::Error);

//
// Actions. Each runs once per tick while its state is current and returns
// where to go next (or stay). None of them may block.
//

// "Wait for <Advance>, then go to <Next>; any fault goes to Recovery."
// Faults are checked first, so a fault wins over a simultaneous Advance.
// &waitFor<...> is a plain function pointer, usable in any table row.
template <EventFlag Advance, StateId Next, std::uint32_t Faults>
Transition waitFor(SessionContext &, EventFlags &events)
{
    if (events.consumeAny(Faults))
    {
        return Transition::go(StateId::Recovery);
    }

    if (events.consume(Advance))
    {
        return Transition::go(Next);
    }

    return Transition::stay();
}

// "Only faults matter" (Running).
template <std::uint32_t Faults>
Transition watchFaults(SessionContext &, EventFlags &events)
{
    return events.consumeAny(Faults) ? Transition::go(StateId::Recovery) : Transition::stay();
}

Transition disconnected(SessionContext &, EventFlags &events)
{
    return events.consume(EventFlag::BleConnected) ? Transition::go(StateId::StartWifi)
                                                   : Transition::stay();
}

Transition startWifi(SessionContext &context, EventFlags &)
{
    context.services.wifi().start();
    context.services.wifi().connect();

    return Transition::stay();
}

Transition startTransport(SessionContext &context, EventFlags &)
{
    context.services.transport().open();

    return Transition::stay();
}

Transition startProtocol(SessionContext &context, EventFlags &)
{
    context.services.protocol().start();

    return Transition::stay();
}

Transition startAuthentication(SessionContext &context, EventFlags &)
{
    context.services.bluetooth().start();
    context.services.bluetooth().pair();

    return Transition::stay();
}

Transition startServices(SessionContext &, EventFlags &)
{
    // TODO: start the Android Auto services (video/audio/input channels).

    return Transition::stay();
}

Transition recovery(SessionContext &context, EventFlags &)
{
    // Order matters: unwind the handshake in the reverse order it was
    // built up in, highest layer first, so each driver is torn down while
    // the layer below it is still available if it needs it.
    context.services.bluetooth().unpair();
    context.services.bluetooth().stop();

    context.services.protocol().stop();

    context.services.transport().close();

    context.services.wifi().disconnect();
    context.services.wifi().stop();

    return Transition::stay();
}

//
// The table. One block per state:
//   - transient states (Start*, Recovery) have `.then(...)`: run once, then
//     move on, no input needed;
//   - waiting states repeat their action every tick until an event they
//     accept shows up. Events they do not accept are dropped.
//

constexpr SessionTable buildTable()
{
    TableBuilder<SessionContext> b;

    b.state(StateId::Disconnected)
        .action(&disconnected)
        .accepts(EventFlag::BleConnected);

    b.state(StateId::StartWifi)
        .action(&startWifi)
        .then(StateId::WaitingWifi);

    b.state(StateId::WaitingWifi)
        .action(&waitFor<EventFlag::WifiConnected, StateId::StartTransport, FaultsBleOnly>)
        .accepts(EventFlag::WifiConnected)
        .acceptsMask(FaultsBleOnly);

    b.state(StateId::StartTransport)
        .action(&startTransport)
        .then(StateId::WaitingTransport);

    b.state(StateId::WaitingTransport)
        .action(&waitFor<EventFlag::TransportReady, StateId::StartProtocol, FaultsAll>)
        .accepts(EventFlag::TransportReady)
        .acceptsMask(FaultsAll);

    b.state(StateId::StartProtocol)
        .action(&startProtocol)
        .then(StateId::WaitingProtocol);

    b.state(StateId::WaitingProtocol)
        .action(&waitFor<EventFlag::PairingRequest, StateId::StartAuthentication, FaultsAll>)
        .accepts(EventFlag::PairingRequest)
        .acceptsMask(FaultsAll);

    b.state(StateId::StartAuthentication)
        .action(&startAuthentication)
        .then(StateId::WaitingAuthentication);

    b.state(StateId::WaitingAuthentication)
        .action(&waitFor<EventFlag::PairingCompleted, StateId::StartServices, FaultsAll>)
        .accepts(EventFlag::PairingCompleted)
        .acceptsMask(FaultsAll);

    b.state(StateId::StartServices)
        .action(&startServices)
        .then(StateId::WaitingServices);

    b.state(StateId::WaitingServices)
        .action(&waitFor<EventFlag::ServicesReady, StateId::Running, FaultsAll>)
        .accepts(EventFlag::ServicesReady)
        .acceptsMask(FaultsAll);

    b.state(StateId::Running)
        .action(&watchFaults<FaultsAll>)
        .acceptsMask(FaultsAll);

    b.state(StateId::Recovery)
        .action(&recovery)
        .then(StateId::Disconnected);

    return b.table();
}

constexpr SessionTable kTable = buildTable();

} // namespace

SessionTable const & sessionTable() noexcept
{
    return kTable;
}

} // namespace aa::session
