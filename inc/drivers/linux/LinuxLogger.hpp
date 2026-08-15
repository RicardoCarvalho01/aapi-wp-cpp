#pragma once

#include <mutex>

#include "services/ILogger.hpp"


namespace aa::drivers::linux
{

class LinuxLogger final : public aa::services::ILogger
{
    public:
        void trace(std::string_view message) override;

        void debug(std::string_view message) override;

        void info(std::string_view message) override;

        void warning(std::string_view message) override;

        void error(std::string_view message) override;

    private:
        std::mutex mutex_;

        void log(std::string_view level, std::string_view message);
};

}