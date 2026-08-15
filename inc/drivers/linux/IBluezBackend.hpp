#pragma once


namespace aa::drivers::linux
{

class IBluezBackend
{
    public:
        virtual ~IBluezBackend() = default;

        IBluezBackend(IBluezBackend const &) = delete;
        IBluezBackend & operator=(IBluezBackend const &) = delete;

        IBluezBackend(IBluezBackend &&) = delete;
        IBluezBackend & operator=(IBluezBackend &&) = delete;

        virtual bool start() = 0;

        virtual void stop() = 0;

        virtual bool pair() = 0;

        virtual void unpair() = 0;

        [[nodiscard]]
        virtual bool paired() const = 0;

    protected:
        IBluezBackend() = default;
};

} // namespace aa::drivers::linux