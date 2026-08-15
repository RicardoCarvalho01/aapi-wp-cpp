
#include <chrono>
#include <thread>

#include "drivers/ble/DummyBleAdapter.hpp"
#include "session/Session.hpp"

int main()
{
    aa::session::Session session;

    aa::drivers::ble::DummyBleAdapter bleAdapter(session.eventQueue());

    if (!bleAdapter.initialize())
    {
        return 1;
    }

    if (!bleAdapter.startAdvertising())
    {
        return 1;
    }

    for (;;)
    {
        bleAdapter.poll();
        session.pump();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}