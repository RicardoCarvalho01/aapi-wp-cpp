#pragma once

#include <cstddef>
#include <span>


namespace aa::services
{

class ITransport
{
    public:
        virtual ~ITransport() = default;

        virtual void open() = 0;

        virtual void close() = 0;

        virtual void send(std::span<std::byte const>) = 0;

        [[nodiscard]]
        virtual bool opened() const = 0;
};

} // namespace aa::services
