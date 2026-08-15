#pragma once


namespace aa::services
{

class IWifi
{
    public:
        virtual ~IWifi() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void connect() = 0;

        virtual void disconnect() = 0;

        [[nodiscard]]
        virtual bool connected() const = 0;
};

} // namespace aa::services
