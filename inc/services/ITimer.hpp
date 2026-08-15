#pragma once

#include <chrono>


namespace aa::services
{

class ITimer
{
    public:
        virtual ~ITimer() = default;

        virtual void start(std::chrono::milliseconds timeout) = 0;

        virtual void stop() = 0;

        [[nodiscard]]
        virtual bool running() const = 0;
};

} // namespace aa::services