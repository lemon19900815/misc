#ifndef __Chain_Logger_Inc_H__
#define __Chain_Logger_Inc_H__

#include "../stdafx.h"

namespace Chain {

constexpr int INFO  = 1;
constexpr int DEBUG = 2;
constexpr int ERROR = 3;

class AbstractLogger {
public:
    virtual ~AbstractLogger() { }
    CLASS_PTR(AbstractLogger);

    void setNextLogger(AbstractLogger::Ptr nextLogger) {
        next_logger_ = nextLogger;
    }

    void logMessage(int level, std::string message) {
        if (level_ <= level) {
            write(message);
        }

        if (next_logger_) {
            next_logger_->logMessage(level, message);
        }
    }

private:
    virtual void write(std::string& message) = 0;

protected:
    int32_t level_;
    AbstractLogger::Ptr next_logger_ = nullptr;
};

class ConsoleLogger : public AbstractLogger {
public:
    ConsoleLogger() {
        level_ = INFO;
    }

    ~ConsoleLogger() { }

private:
    void write(std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }
};

class DebugLogger : public AbstractLogger {
public:
    DebugLogger() {
        level_ = DEBUG;
    }

    ~DebugLogger() { }

private:
    void write(std::string& message) {
        std::cout << "DEBUG: " << message << std::endl;
    }
};

class ErrorLogger : public AbstractLogger {
public:
    ErrorLogger() {
        level_ = ERROR;
    }

    ~ErrorLogger() { }

private:
    void write(std::string& message) {
        std::cout << "ERROR: " << message << std::endl;
    }
};

void test() {
    std::cout << "\n\n chain of responsibility pattern." << std::endl;

    auto infoLogger = std::make_shared<ConsoleLogger>();
    auto debugLogger = std::make_shared<DebugLogger>();
    auto errorLogger = std::make_shared<ErrorLogger>();

    errorLogger->setNextLogger(debugLogger);
    debugLogger->setNextLogger(infoLogger);

    errorLogger->logMessage(ERROR, "failed to find a");
    errorLogger->logMessage(DEBUG, "failed to find b");
    errorLogger->logMessage(INFO,  "failed to find c");
}

}

#endif //  __Chain_Logger_Inc_H__
