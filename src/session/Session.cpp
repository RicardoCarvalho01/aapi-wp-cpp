
#include "session/Session.hpp"

#include <utility>

namespace aa::session
{

Session::Session() : machine_(*this), dispatcher_(eventQueue_, machine_)
{
    startWifi_.setServices(&services_);
    startTransport_.setServices(&services_);
    startProtocol_.setServices(&services_);
    startAuthentication_.setServices(&services_);
    recovery_.setServices(&services_);

    machine_.setInitialState(disconnected_);
}

void Session::dispatch(const aa::core::Event& event)
{
    machine_.dispatch(event);
}

void Session::enqueue(aa::core::Event event)
{
    eventQueue_.push(std::move(event));
}

void Session::pump()
{
    dispatcher_.dispatchAll();
}

aa::core::EventQueue& Session::eventQueue() noexcept
{
    return eventQueue_;
}

aa::core::State* Session::currentState() noexcept
{
    return machine_.currentState();
}

aa::services::IServices& Session::services() noexcept
{
    return services_;
}

aa::core::State& Session::disconnectedState() noexcept
{
    return disconnected_;
}

aa::core::State& Session::startWifiState() noexcept
{
    return startWifi_;
}

aa::core::State& Session::waitingWifiState() noexcept
{
    return waitingWifi_;
}

aa::core::State& Session::startTransportState() noexcept
{
    return startTransport_;
}

aa::core::State& Session::waitingTransportState() noexcept
{
    return waitingTransport_;
}

aa::core::State& Session::startProtocolState() noexcept
{
    return startProtocol_;
}

aa::core::State& Session::waitingProtocolState() noexcept
{
    return waitingProtocol_;
}

aa::core::State& Session::startAuthenticationState() noexcept
{
    return startAuthentication_;
}

aa::core::State& Session::waitingAuthenticationState() noexcept
{
    return waitingAuthentication_;
}

aa::core::State& Session::startServicesState() noexcept
{
    return startServices_;
}

aa::core::State& Session::waitingServicesState() noexcept
{
    return waitingServices_;
}

aa::core::State& Session::runningState() noexcept
{
    return running_;
}

aa::core::State& Session::recoveryState() noexcept
{
    return recovery_;
}

} // namespace aa::session
