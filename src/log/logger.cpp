#include "log/logger.h"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace simpleengine::log {
    void LoggerManager::init() {
        spdlog::set_pattern("[%T %D] [thread %t] [%n] [%l] %v");

        core_logger = std::make_shared<Logger>("engine");

        spdlog::set_default_logger(core_logger->get_inner());
        set_level(spdlog::level::info);
    }

    void LoggerManager::set_level(spdlog::level::level_enum lvl) {
        spdlog::set_level(lvl);
    }

    std::shared_ptr<Logger> LoggerManager::core_logger;

    std::shared_ptr<Logger> LoggerManager::get_core_logger() {
        return core_logger;
    }

    Logger LoggerManager::create_logger(std::string name) {
        return Logger(name);
    }

    Logger LoggerManager::get_logger(std::string name) {
        return Logger(spdlog::get(name));
    }
}