
#include "drivers/linux/LinuxLogger.hpp"

#include <iostream>

namespace aa::drivers::linux
{

void LinuxLogger::trace(std::string_view message)
{
    log("TRACE", message);
}

void LinuxLogger::debug(std::string_view message)
{
    log("DEBUG", message);
}

void LinuxLogger::info(std::string_view message)
{
    log("INFO ", message);
}

void LinuxLogger::warning(std::string_view message)
{
    log("WARN ", message);
}

void LinuxLogger::error(std::string_view message)
{
    log("ERROR", message);
}

void LinuxLogger::log(std::string_view level, std::string_view message)
{
    std::lock_guard lock(mutex_);

    std::cout << '[' << level << "] " << message << '\n';
}

} // namespace aa::drivers::linux