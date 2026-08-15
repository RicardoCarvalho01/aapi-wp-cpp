#pragma once

#include <string_view>


namespace aa::services
{

class ILogger
{
    public:
        virtual ~ILogger() = default;

        virtual void trace(std::string_view message) = 0;

        virtual void debug(std::string_view message) = 0;

        virtual void info(std::string_view message) = 0;

        virtual void warning(std::string_view message) = 0;

        virtual void error(std::string_view message) = 0;
};

} // namespace aa::services