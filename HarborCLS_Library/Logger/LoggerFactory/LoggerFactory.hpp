//
// Created by josephvalverde on 12/14/23.
//

#ifndef HARBOR_CLS_LOGGERFACTORY_HPP
#define HARBOR_CLS_LOGGERFACTORY_HPP

#include <functional>
#include <unordered_map>
#include <tuple>

#include "ILoggerFactory.hpp"
#include "../LoggerConfiguration.hpp"

#include "../LoggerBuilder/BasicLoggerBuilder.hpp"

namespace HarborCLS {

  template<typename Builder = BasicLoggerBuilder>
  class LoggerFactory : public ILoggerFactory {
    LoggerConfiguration _loggerConfiguration{};
    Builder _loggerBuilder{};

  public:
    LoggerFactory() = default;

    /**
     * @brief Create a logger factory with a logger configuration.
     * @param loggerConfiguration The logger configuration.
     */
    explicit LoggerFactory(LoggerConfiguration loggerConfiguration)
        : _loggerConfiguration(std::move(loggerConfiguration)) {
    }

    ~LoggerFactory() override = default;

    /**
     * @brief Create a shared logger.
     * @return A shared logger.
     */
    [[nodiscard]] std::shared_ptr<ILogger> createLogger() override {
      _loggerBuilder.setConfiguration(_loggerConfiguration);

      std::shared_ptr<ILogger> logger(_loggerBuilder.build());

      _loggerBuilder.reset();
      return logger;
    }

    /**
     * @brief Create a unique logger.
     * @return A unique logger.
     */
    [[nodiscard]] std::unique_ptr<ILogger> createUniqueLogger() override {
      if (_loggerConfiguration.sharedLog) {
        throw std::runtime_error("Cannot create unique logger from shared logger configuration.");
      }

      _loggerBuilder.setConfiguration(_loggerConfiguration);

      std::unique_ptr<ILogger> logger (_loggerBuilder.build());

      _loggerBuilder.reset();
      return logger;
    }
  };
}

#endif //HARBOR_CLS_LOGGERFACTORY_HPP