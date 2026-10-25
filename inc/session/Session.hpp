#pragma once

#include <atomic>
#include <chrono>
#include <stop_token>

#include "core/events/EventFlags.hpp"
#include "core/state/StateId.hpp"
#include "services/IServices.hpp"

#include "session/SessionContext.hpp"


namespace aa::session
{

// The main task: the Android Auto handshake FSM.
//
// There is no dispatcher and no queue. Drivers raise event flags with
// events().set(...) from wherever they run. encodeFSM() runs the current
// state's action once and moves to the next state; it never blocks. run() is
// the main task loop: it calls encodeFSM() and sleeps, over and over.
//
// The services (USB, wifi, bluetooth, transport, protocol, ...) are
// injected, so Session does not depend on any concrete driver. The services
// object must outlive the Session.
class Session final
{
    public:
        explicit Session(aa::services::IServices &services);

        Session(Session const &) = delete;

        Session & operator=(Session const &) = delete;

        // One non-blocking step of the FSM: runs the action of the current
        // state and updates the current state. Call it from a single
        // thread only (normally via run()).
        void encodeFSM();

        // The main task loop: encodeFSM(), then sleep until the next tick,
        // until a stop is requested. Ticks stay on a fixed grid (no drift);
        // if an action overruns, the missed ticks are skipped. Stopping takes
        // at most one period.
        //
        //   session.run(std::stop_token{});   // main follows the FSM
        void run(std::stop_token stop,
                 std::chrono::milliseconds period = std::chrono::milliseconds(10));

        // Raise events here (thread-safe).
        [[nodiscard]]
        aa::core::EventFlags & events() noexcept;

        // Safe to read from any thread.
        [[nodiscard]]
        aa::core::StateId state() const noexcept;

        [[nodiscard]]
        aa::services::IServices & services() noexcept;

    private:
        SessionContext context_;

        aa::core::EventFlags events_;

        std::atomic<aa::core::StateId> state_{aa::core::StateId::Disconnected};
};

}
