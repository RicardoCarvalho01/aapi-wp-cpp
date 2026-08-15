#pragma once

#include "core/state/IStateContext.hpp"
#include "core/state/State.hpp"

namespace aa::test
{

// A minimal, dependency-free stand-in for aa::session::Session.
// Each accessor returns a distinct plain aa::core::State instance,
// so tests can assert *which* state a transition targeted just by
// comparing pointers (e.g. &machine.currentState() == &context.startWifi).
class MockStateContext : public aa::core::IStateContext
{
public:
    aa::core::State& disconnectedState() override { return disconnected; }

    aa::core::State& startWifiState() override { return startWifi; }

    aa::core::State& waitingWifiState() override { return waitingWifi; }

    aa::core::State& startTransportState() override { return startTransport; }

    aa::core::State& waitingTransportState() override { return waitingTransport; }

    aa::core::State& startProtocolState() override { return startProtocol; }

    aa::core::State& waitingProtocolState() override { return waitingProtocol; }

    aa::core::State& startAuthenticationState() override { return startAuthentication; }

    aa::core::State& waitingAuthenticationState() override { return waitingAuthentication; }

    aa::core::State& startServicesState() override { return startServices; }

    aa::core::State& waitingServicesState() override { return waitingServices; }

    aa::core::State& runningState() override { return running; }

    aa::core::State& recoveryState() override { return recovery; }

    aa::core::State disconnected;
    aa::core::State startWifi;
    aa::core::State waitingWifi;
    aa::core::State startTransport;
    aa::core::State waitingTransport;
    aa::core::State startProtocol;
    aa::core::State waitingProtocol;
    aa::core::State startAuthentication;
    aa::core::State waitingAuthentication;
    aa::core::State startServices;
    aa::core::State waitingServices;
    aa::core::State running;
    aa::core::State recovery;
};

} // namespace aa::test
