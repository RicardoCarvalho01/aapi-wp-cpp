#pragma once

#include <atomic>
#include <cstdint>


namespace aa::core
{

// An event is just a bit. Anyone (a driver callback, another thread, a test)
// raises it with EventFlags::set(); the FSM looks at it on its next tick.
//
// If an event carries data, put the data in a slot next to the flag (an
// atomic, or a plain variable guarded by the flag itself) and write it
// *before* set(): set() is a release, consume() is an acquire, so whoever
// consumes the flag also sees the data.
enum class EventFlag : std::uint32_t
{
    BleConnected = 1u << 0,
    BleDisconnected = 1u << 1,
    WifiConnected = 1u << 2,
    WifiDisconnected = 1u << 3,
    TransportReady = 1u << 4,
    PairingRequest = 1u << 5,
    PairingCompleted = 1u << 6,
    ServicesReady = 1u << 7,
    Error = 1u << 8,
};

[[nodiscard]]
constexpr std::uint32_t bit(EventFlag flag) noexcept
{
    return static_cast<std::uint32_t>(flag);
}

// Bit mask of several flags: maskOf(EventFlag::Error, EventFlag::WifiConnected).
template <typename... Flags>
[[nodiscard]]
constexpr std::uint32_t maskOf(Flags... flags) noexcept
{
    return (std::uint32_t{0} | ... | bit(flags));
}

// A set of pending event flags. Lock-free; every operation is a single
// atomic read-modify-write, so it is safe to raise events from any thread
// while the FSM consumes them from another.
//
// Raising the same flag twice before it is consumed counts once: a flag
// says "this happened", not "this happened N times".
class EventFlags
{
    public:
        // Raise an event. Never blocks.
        void set(EventFlag flag) noexcept
        {
            bits_.fetch_or(bit(flag), std::memory_order_release);
        }

        [[nodiscard]]
        bool test(EventFlag flag) const noexcept
        {
            return (bits_.load(std::memory_order_acquire) & bit(flag)) != 0;
        }

        // Atomically tests and clears one flag; true if it was raised.
        [[nodiscard]]
        bool consume(EventFlag flag) noexcept
        {
            return consumeAny(bit(flag));
        }

        // Atomically clears every flag in `mask`; true if any was raised.
        [[nodiscard]]
        bool consumeAny(std::uint32_t mask) noexcept
        {
            return (bits_.fetch_and(~mask, std::memory_order_acq_rel) & mask) != 0;
        }

        // Drops every pending flag that is NOT in `keep` (events that make
        // no sense right now). Returns the mask of what was dropped, in
        // case the caller wants to log or evaluate it.
        std::uint32_t clearExcept(std::uint32_t keep) noexcept
        {
            return bits_.fetch_and(keep, std::memory_order_acq_rel) & ~keep;
        }

        [[nodiscard]]
        std::uint32_t pending() const noexcept
        {
            return bits_.load(std::memory_order_acquire);
        }

        void clear() noexcept
        {
            bits_.store(0, std::memory_order_release);
        }

    private:
        std::atomic<std::uint32_t> bits_{0};
};

}
