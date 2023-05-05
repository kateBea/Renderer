#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Kate {
    class Logger {
    public:
        static auto Init() -> void;

        static auto GetLogger() -> const std::shared_ptr<spdlog::logger>&;


    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}

// Log macros
#define KATE_LOGGER_ERROR(...) Kate::Logger::getLogger()->error(__VA_ARGS__);
#define KATE_LOGGER_WARN(...) Kate::Logger::getLogger()->warn(__VA_ARGS__);
#define KATE_LOGGER_CRITICAL(...) Kate::Logger::getLogger()->critical(__VA_ARGS__);
#define KATE_LOGGER_TRACE(...) Kate::Logger::getLogger()->trace(__VA_ARGS__);
#define KATE_LOGGER_INFO(...) Kate::Logger::getLogger()->info(__VA_ARGS__);
#define KATE_LOGGER_DEBUG(...) Kate::Logger::getLogger()->debug(__VA_ARGS__);


#endif // LOGGER_H
