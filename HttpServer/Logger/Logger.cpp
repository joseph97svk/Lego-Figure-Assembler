//
// Created by josephvalverde on 12/15/23.
//

#include "Logger.hpp"

Logger::Logger (std::shared_ptr<ILoggerBufferingPolicy> bufferingStrategy,
                std::shared_ptr<ILoggerFileManagementPolicy> fileManagementStrategy,
                std::shared_ptr<ILogFileRotation> fileRotationStrategy,
                std::string& logFilePath)
        : _bufferingStrategy(std::move(bufferingStrategy))
        , _fileManagementStrategy(std::move(fileManagementStrategy))
        , _fileRotationStrategy(std::move(fileRotationStrategy))
        , _logFile(_fileManagementStrategy->getLogFile(logFilePath)) {}


void Logger::log(std::string message) {
  const std::string messageType = "Log";

  this->logMessage(messageType, message);
}

void Logger::warning(std::string message) {
  const std::string messageType = "Warning";

  this->logMessage(messageType, message);
}

void Logger::error(std::string message) {
  const std::string messageType = "Error";

  this->logMessage(messageType, message);
}

void Logger::info(std::string message) {
  const std::string messageType = "Info";

  this->logMessage(messageType, message);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void Logger::logMessage(const std::string& messageType, std::string& message) {
  _logFutures.push_back(std::async([this, message, messageType]() {
    std::string completedMessage = getCurrentTime() + " [ " + messageType + " ] : " + message + "\n";
    std::optional<std::string> bufferedMessage = _bufferingStrategy->buffer(completedMessage);

    if (!bufferedMessage.has_value()) return;

    std::optional<std::ofstream> fileStream;

    std::visit(overloaded {
      [&fileStream](std::ofstream& ofs) {
        fileStream = std::move(ofs);
      },
      [&fileStream](const std::string& str) {
        fileStream = std::nullopt;
      }
    }, _logFile);

    _fileManagementStrategy->log(bufferedMessage.value(), fileStream, _canWrite);
  }));
}