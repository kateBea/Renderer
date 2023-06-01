#include "Core/Logger.hh"

namespace kT {
    // Don't forget to call this function before using the logger,
    // or you will get null pointer read exceptions
    auto Logger::Init() -> void {
        logger = spdlog::stdout_color_mt("KATE_CORE_LOGGER");

        // Set logger pattern. Check out spdlog wiki for info about formatting
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        logger->set_pattern("%^[%X] [thread %t] %v%$");
        logger->set_level(spdlog::level::trace);

        // Auto flush when "debug" or higher level message is logged on all loggers.
        // Check the FAQ for more about this matter.
        // https://github.com/gabime/spdlog/wiki/0.-FAQ
        spdlog::flush_on(spdlog::level::debug);
    }

    auto Logger::GetLogger() -> const std::shared_ptr<spdlog::logger>& {
        return logger;
    }
}