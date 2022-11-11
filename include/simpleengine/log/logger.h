#pragma once

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
        /**
         * @brief Construct a new Logger object from a name
         * 
         * This will first check if the logger exists, if it does not it will create a new `color_mt` logger
         * 
         * @param name The name of the logger.
         */
        Logger(std::string name) {
            inner = spdlog::get(name);

            if (!inner) {
                inner = spdlog::stderr_color_mt(name);
            }
        }

        /**
         * @brief Construct a new Logger object using a spdlog::logger
         * 
         * @param logger The inner spdlog::logger that this object will use
         */
        Logger(std::shared_ptr<spdlog::logger> logger) : inner(logger) {

        }

        /**
         * @brief Get the inner spdlog logger object
         * 
         * @return std::shared_ptr<spdlog::logger> 
         */
        std::shared_ptr<spdlog::logger> get_inner() {
            return inner;
        }

        /**
         * @brief Set the log level.
         * 
         * @param lvl The level to set the log to
         */
        void set_level(spdlog::level::level_enum lvl) {
            inner->set_level(lvl);
        }

        /**
         * @brief Log a message.
         * 
         * @param lvl Level to log at
         * @param msg The message to log
         */
        void log(spdlog::level::level_enum lvl, spdlog::string_view_t msg) {
            inner->log(spdlog::source_loc{}, lvl, msg);
        }

        /**
         * @brief Log a formatted message.
         * 
         * @tparam Args 
         * @param lvl Level to log at
         * @param fmt The string that will be formatted
         * @param args The arguments to format into the message.
         */
        template<typename... Args>
        void log(spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt, Args &&...args)
        {
            inner->log(spdlog::source_loc{}, lvl, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log a trace message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void trace(const T &msg) {
            log(spdlog::level::trace, msg);
        }

        /**
         * @brief Log a debug message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void debug(const T &msg) {
            log(spdlog::level::debug, msg);
        }

        /**
         * @brief Log an info message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void info(const T &msg) {
            log(spdlog::level::info, msg);
        }

        /**
         * @brief Log a warning message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void warn(const T &msg) {
            log(spdlog::level::warn, msg);
        }

        /**
         * @brief Log an error message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void error(const T &msg) {
            log(spdlog::level::err, msg);
        }

        /**
         * @brief Log a critical message.
         * 
         * @tparam T 
         * @param msg The message that will be logged.
         */
        template<typename T>
        void critical(const T &msg) {
            log(spdlog::level::critical, msg);
        }

        /**
         * @brief Log a trace message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void trace(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log a debug message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void debug(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log an info message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void info(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::info, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log a warning message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void warn(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log an error message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void error(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::err, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief Log a critical message.
         * 
         * @tparam Args 
         * @param fmt String that will be formatted
         * @param args Arguments to format into the message
         */
        template<typename... Args>
        void critical(fmt::format_string<Args...> fmt, Args &&... args) {
            log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
        }
    };

    using LoggerPtr = std::shared_ptr<Logger>;

    class LoggerManager {
    private:
        static std::shared_ptr<Logger> core_logger;
    public:
        /**
         * @brief Initialize the logger.
         * 
         */
        static void init();

        /**
         * @brief Set the core log level.
         * 
         * @param lvl 
         */
        static void set_level(spdlog::level::level_enum lvl);

        /**
         * @brief Get the core logger.
         * 
         * @return std::shared_ptr<Logger> 
         */
        static std::shared_ptr<Logger> get_core_logger();

        /**
         * @brief Create a new logger.
         * 
         * @param name The name of the logger
         * @return Logger 
         */
        static Logger create_logger(std::string name);

        /**
         * @brief Get a logger using its name.
         * 
         * @param name The name of the logger
         * @return Logger 
         */
        static Logger get_logger(std::string name);
    };
}

// Log macros

/**
 * @brief Log to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param level The level to log at.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_LOG(logger_name, level, message, ...) simpleengine::log::LoggerManager::get_logger(logger_name).log(level, fmt::format(message, ##__VA_ARGS__));

/**
 * @brief Log to the core logger.
 * 
 * @param level The level to log at.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CLOG(level, message, ...) simpleengine::log::LoggerManager::get_core_logger()->log(level, fmt::format(message, ##__VA_ARGS__));

/**
 * @brief Log a critical message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CRIT(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::critical, message, ##__VA_ARGS__);

/**
 * @brief Log a critical message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CCRIT(message, ...) SE_CLOG(spdlog::level::critical, message, ##__VA_ARGS__);

/**
 * @brief Log an error message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_ERROR(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::err, message, ##__VA_ARGS__);

/**
 * @brief Log an error message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CERROR(message, ...) SE_CLOG(spdlog::level::err, message, ##__VA_ARGS__);

/**
 * @brief Log a warning message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_WARN(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::warn, message, ##__VA_ARGS__);

/**
 * @brief Log a warning message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CWARN(message, ...) SE_CLOG(spdlog::level::warn, message, ##__VA_ARGS__);

/**
 * @brief Log an info message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_INFO(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::info, message, ##__VA_ARGS__);

/**
 * @brief Log an info message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CINFO(message, ...) SE_CLOG(spdlog::level::info, message, ##__VA_ARGS__);

/**
 * @brief Log a debug message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_DEBUG(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::debug, message, ##__VA_ARGS__);

/**
 * @brief Log a debug message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CDEBUG(message, ...) SE_CLOG(spdlog::level::debug, message, ##__VA_ARGS__);

/**
 * @brief Log a trace message to a logger.
 * 
 * @param logger_name The name of the logger to log to.
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_TRACE(logger_name, message, ...) SE_LOG(logger_name, spdlog::level::trace, message, ##__VA_ARGS__);

/**
 * @brief Log a trace message to the core logger.
 * 
 * @param message The (to be) formatted message to log.
 * @param ... The variables that will be formatted in the text.
 */
#define SE_CTRACE(message, ...) SE_CLOG(spdlog::level::trace, message, ##__VA_ARGS__);