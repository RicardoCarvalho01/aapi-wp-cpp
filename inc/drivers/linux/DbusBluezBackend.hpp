#pragma once

#include <memory>
#include <string>

#include "drivers/linux/IBluezBackend.hpp"


namespace aa::drivers::linux
{

class DbusBluezBackend final : public IBluezBackend
{
    public:
        DbusBluezBackend();

        ~DbusBluezBackend() override;

        DbusBluezBackend(DbusBluezBackend const &) = delete;
        DbusBluezBackend & operator=(DbusBluezBackend const &) = delete;

        DbusBluezBackend(DbusBluezBackend &&) = delete;
        DbusBluezBackend & operator=(DbusBluezBackend &&) = delete;

        bool start() override;

        void stop() override;

        bool pair() override;

        void unpair() override;

        [[nodiscard]]
        bool paired() const override;

    private:
        class Impl;

        std::unique_ptr<Impl> impl_;
};

} // namespace aa::drivers::linux