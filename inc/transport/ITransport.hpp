#pragma once

#include "transport/Packet.hpp"


namespace aa::transport
{

class ITransport
{
    public:
        virtual ~ITransport() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        [[nodiscard]]
        virtual bool connected() const noexcept = 0;

        virtual void send(Packet const &packet) = 0;
};

}