#pragma once

#include <chrono>


namespace aa::services
{

class IClock
{
    public:
        virtual ~IClock() = default;

        [[nodiscard]]
        virtual std::chrono::steady_clock::time_point now() const = 0;
};

} // namespace aa::services