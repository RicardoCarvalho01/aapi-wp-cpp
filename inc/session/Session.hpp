#pragma once

#include "core/events/Event.hpp"
#include "core/events/EventDispatcher.hpp"
#include "core/events/EventQueue.hpp"
#include "core/state/IStateContext.hpp"
#include "core/state/StateMachine.hpp"

#include "session/states/DisconnectedState.hpp"
#include "session/states/RecoveryState.hpp"
#include "session/states/RunningState.hpp"
#include "session/states/StartAuthenticationState.hpp"
#include "session/states/StartProtocolState.hpp"
#include "session/states/StartServicesState.hpp"
#include "session/states/StartWifiState.hpp"
#include "session/states/WaitingAuthenticationState.hpp"
#include "session/states/WaitingProtocolState.hpp"
#include "session/states/WaitingServicesState.hpp"
#include "session/states/WaitingTransportState.hpp"
#include "session/states/WaitingWifiState.hpp"
#include "session/states/StartTransportState.hpp"

#include "drivers/linux/LinuxServices.hpp"


namespace aa::session
{

// Owns the full Android Auto handshake state graph (13 states) and the set
// of Linux-backed services (USB, wifi, bluetooth, transport, protocol,
// timer, clock, logger) those states will eventually drive.
//
// Session is deliberately default-constructible: it owns everything it
// needs, which keeps `Session session;` enough to get a working, fully
// wired state machine (see tests/session/SessionTest.cpp).
class Session final : public aa::core::IStateContext
{
    public:
        Session();

        // Routes a single event to the state machine synchronously. Use
        // this when the caller is already on the session's own thread of
        // control (e.g. in a test).
        void dispatch(aa::core::Event const &event);

        // Queues an event for later processing via pump(). This is the
        // entry point for asynchronous producers (BLE callbacks, driver
        // threads, ...) that don't run on the session's own thread.
        void enqueue(aa::core::Event event);

        // Drains every event queued via enqueue() into the state machine.
        // Call this once per iteration of the owning main loop.
        void pump();

        [[nodiscard]]
        aa::core::EventQueue & eventQueue() noexcept;

        [[nodiscard]]
        aa::core::State * currentState() noexcept;

        [[nodiscard]]
        aa::services::IServices & services() noexcept;

        [[nodiscard]]
        aa::core::State & disconnectedState() noexcept override;

        [[nodiscard]]
        aa::core::State & startWifiState() noexcept override;

        [[nodiscard]]
        aa::core::State & waitingWifiState() noexcept override;

        [[nodiscard]]
        aa::core::State & startTransportState() noexcept override;

        [[nodiscard]]
        aa::core::State & waitingTransportState() noexcept override;

        [[nodiscard]]
        aa::core::State & startProtocolState() noexcept override;

        [[nodiscard]]
        aa::core::State & waitingProtocolState() noexcept override;

        [[nodiscard]]
        aa::core::State & startAuthenticationState() noexcept override;

        [[nodiscard]]
        aa::core::State & waitingAuthenticationState() noexcept override;

        [[nodiscard]]
        aa::core::State & startServicesState() noexcept override;

        [[nodiscard]]
        aa::core::State & waitingServicesState() noexcept override;

        [[nodiscard]]
        aa::core::State & runningState() noexcept override;

        [[nodiscard]]
        aa::core::State & recoveryState() noexcept override;

    private:
        aa::drivers::linux::LinuxServices services_;

        DisconnectedState disconnected_;
        StartWifiState startWifi_;
        WaitingWifiState waitingWifi_;
        StartTransportState startTransport_;
        WaitingTransportState waitingTransport_;
        StartProtocolState startProtocol_;
        WaitingProtocolState waitingProtocol_;
        StartAuthenticationState startAuthentication_;
        WaitingAuthenticationState waitingAuthentication_;
        StartServicesState startServices_;
        WaitingServicesState waitingServices_;
        RunningState running_;
        RecoveryState recovery_;

        aa::core::StateMachine machine_;

        aa::core::EventQueue eventQueue_;
        aa::core::EventDispatcher dispatcher_;
};

}
