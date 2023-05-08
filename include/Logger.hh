#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kT {
    class Logger {
    public:
        /**
         * Initializes the Logger. Don't forget to call this function before
         * using any of the available methods
         * */
        static auto Init() -> void;

        static auto GetLogger() -> const std::shared_ptr<spdlog::logger>&;


    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}

// Log macros
#define KATE_LOGGER_ERROR(...) kT::Logger::GetLogger()->error(__VA_ARGS__)
#define KATE_LOGGER_WARN(...) kT::Logger::GetLogger()->warn(__VA_ARGS__)
#define KATE_LOGGER_CRITICAL(...) kT::Logger::GetLogger()->critical(__VA_ARGS__)
#define KATE_LOGGER_TRACE(...) kT::Logger::GetLogger()->trace(__VA_ARGS__)
#define KATE_LOGGER_INFO(...) kT::Logger::GetLogger()->info(__VA_ARGS__)
#define KATE_LOGGER_DEBUG(...) kT::Logger::GetLogger()->debug(__VA_ARGS__)


#endif // LOGGER_H
