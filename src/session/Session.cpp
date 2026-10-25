
#include "session/Session.hpp"

#include <thread>

#include "core/state/EncodeFsm.hpp"

namespace aa::session
{

Session::Session(aa::services::IServices& services) : context_{services}
{}

void Session::encodeFSM()
{
    state_ = aa::core::encodeFSM(sessionTable(), state_.load(), context_, events_);
}

void Session::run(std::stop_token stop, std::chrono::milliseconds period)
{
    auto next = std::chrono::steady_clock::now();

    while (!stop.stop_requested())
    {
        encodeFSM();

        next += period;

        if (auto const now = std::chrono::steady_clock::now(); next < now)
        {
            // Overran: skip the ticks we missed, keep the grid.
            next += period * ((now - next) / period + 1);
        }

        std::this_thread::sleep_until(next);
    }
}

aa::core::EventFlags& Session::events() noexcept
{
    return events_;
}

aa::core::StateId Session::state() const noexcept
{
    return state_;
}

aa::services::IServices& Session::services() noexcept
{
    return context_.services;
}

} // namespace aa::session
