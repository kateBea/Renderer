#include "Logger.hh"

namespace Kate {

    // Initialize logger
    std::shared_ptr<spdlog::logger> Logger::logger{};

    auto Logger::init() -> void {
        logger = spdlog::stdout_color_mt("Kate Logger");

        // Set logger pattern. Check out spdlog wiki for info about formatting
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        logger->set_pattern("%^[%X] [thread %t] %v%$");
        logger->set_level(spdlog::level::trace);
    }

    auto Logger::getLogger() -> const std::shared_ptr<spdlog::logger> & {
        return logger;
    }
}