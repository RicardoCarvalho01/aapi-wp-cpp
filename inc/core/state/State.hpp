#pragma once

#include "core/events/Event.hpp"


namespace aa::core
{

class StateMachine;

// Base class for every state in the session state machine.
//
// A state reacts to lifecycle callbacks (onEnter/onExit) and to individual
// event types via the onEvent overloads below. Every overload has a no-op
// default, so a concrete state only needs to override what it actually
// cares about; everything else is silently ignored, matching the "unhandled
// events are ignored" behaviour the state machine relies on.
//
// aa::core::State is intentionally concrete (not abstract): tests use plain
// State instances as opaque, comparable markers (see MockStateContext), and
// real states such as aa::session::DisconnectedState derive from it and
// override only the handlers relevant to that stage of the handshake.
class State
{
    public:
        virtual ~State() = default;

        virtual void onEnter(StateMachine &)
        { }

        virtual void onExit(StateMachine &)
        { }

        virtual void onEvent(StateMachine &, BleConnectedEvent const &)
        { }

        virtual void onEvent(StateMachine &, BleDisconnectedEvent const &)
        { }

        virtual void onEvent(StateMachine &, WifiConnectedEvent const &)
        { }

        virtual void onEvent(StateMachine &, WifiDisconnectedEvent const &)
        { }

        virtual void onEvent(StateMachine &, TransportReadyEvent const &)
        { }

        virtual void onEvent(StateMachine &, PairingRequestEvent const &)
        { }

        virtual void onEvent(StateMachine &, PairingCompletedEvent const &)
        { }

        virtual void onEvent(StateMachine &, ServicesReadyEvent const &)
        { }

        virtual void onEvent(StateMachine &, ErrorEvent const &)
        { }
};

}
