#pragma once


namespace aa::services
{

class IBluetooth
{
    public:
        virtual ~IBluetooth() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void pair() = 0;

        virtual void unpair() = 0;

        [[nodiscard]]
        virtual bool paired() const = 0;
};

} // namespace aa::services
