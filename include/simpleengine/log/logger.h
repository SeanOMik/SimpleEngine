//#define SPDLOG_FMT_EXTERNAL

#include <memory>
#include <string>
#include <unordered_map>

#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace simpleengine::log {
    class Logger {
    private:
        std::shared_ptr<spdlog::logger> inner;
    public:
        Logger(std::string name) {
            inner = spdlog::get(name);

            if (!inner) {
                inner = spdlog::stderr_color_mt(name);
            }
        }

        Logger(std::shared_ptr<spdlog::logger> logger) : inner(logger) {

        }

        std::shared_ptr<spdlog::logger> get_inner() {
            return inner;
        }

        void set_level(spdlog::level::level_enum lvl) {
            inner->set_level(lvl);
        }

        void log(spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
            inner->log(spdlog::source_loc{}, lvl, msg);
        }

        template<typename T>
        void trace(const T &msg) {
            log(spdlog::level::trace, msg);
        }

        template<typename T>
        void debug(const T &msg) {
            log(spdlog::level::debug, msg);
        }

        template<typename T>
        void info(const T &msg) {
            log(spdlog::level::info, msg);
        }

        template<typename T>
        void warn(const T &msg) {
            log(spdlog::level::warn, msg);
        }

        template<typename T>
        void error(const T &msg) {
            log(spdlog::level::err, msg);
        }

        template<typename T>
        void critical(const T &msg) {
            log(spdlog::level::critical, msg);
        }

        template<typename... Args>
        void trace(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void info(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::info, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void error(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::err, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
        }
    };

    using LoggerPtr = std::shared_ptr<Logger>;

    class LoggerManager {
    private:
        static std::shared_ptr<Logger> core_logger;
    public:
        /* LoggerManager() {
            spdlog::set_pattern("[%R - %D] [%l] [thread %t] [%n] %v");

            core_logger = std::make_shared<Logger>("Engine");
        } */

        static void init();
        static void set_level(spdlog::level::level_enum lvl);

        static std::shared_ptr<Logger> get_core_logger();

        static Logger create_logger(std::string name);
        static Logger get_logger(std::string name);
    };
}