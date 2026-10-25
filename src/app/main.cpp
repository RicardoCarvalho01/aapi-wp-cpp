
#include <chrono>
#include <stop_token>
#include <thread>

#include "drivers/ble/DummyBleAdapter.hpp"
#include "session/Session.hpp"

#if defined(AAPIWP_LINUX_DRIVERS)
#include "drivers/linux/LinuxServices.hpp"
#else
#include "services/NullServices.hpp"
#endif /* AAPIWP_LINUX_DRIVERS! */


int main()
{
#if defined(AAPIWP_LINUX_DRIVERS)
    aa::drivers::linux::LinuxServices services;
#else
    aa::services::NullServices services;
#endif /* AAPIWP_LINUX_DRIVERS! */

    // The main task: the FSM. Everything else only raises event flags.
    aa::session::Session session(services);

    aa::drivers::ble::DummyBleAdapter bleAdapter( session.events() );

    if ( !bleAdapter.initialize() ) {
        return 1;
    }

    if ( !bleAdapter.startAdvertising() ) {
        return 1;
    }

    // Side job: poll the BLE adapter. It only raises event flags, which
    // are thread-safe, so it can run on its own thread. Declared last, so it
    // is stopped and joined first on exit.
    std::jthread bleThread(
        [&bleAdapter](std::stop_token stop)
        {
            while ( !stop.stop_requested() ) {
                bleAdapter.poll();
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );
            }
        }
    );

    // The main task: main *is* the FSM. encodeFSM(), sleep, repeat (every
    // 10 ms). Returns only if a stop is requested; none is wired up yet, so
    // it runs until the process is killed.
    session.run( std::stop_token{}, std::chrono::milliseconds(10) );

    return 0;
}
