#pragma once


namespace aa::services
{

class IUsb
{
    public:
        virtual ~IUsb() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        [[nodiscard]]
        virtual bool connected() const = 0;
};

} // namespace aa::services