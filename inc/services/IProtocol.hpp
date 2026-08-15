#pragma once


namespace aa::services
{

class IProtocol
{
    public:
        virtual ~IProtocol() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void reset() = 0;

        [[nodiscard]]
        virtual bool running() const = 0;
};

} // namespace aa::services
