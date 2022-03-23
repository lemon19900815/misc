#ifndef __Chain_Logger_Inc_H__
#define __Chain_Logger_Inc_H__

#include "../stdafx.h"

namespace Chain
{
	constexpr int INFO  = 1;
	constexpr int DEBUG = 2;
	constexpr int ERROR = 3;

	class AbstractLogger
	{
	public:
		virtual ~AbstractLogger() {

		}

		void setNextLogger(AbstractLogger* nextLogger) {
			nextLogger_ = nextLogger;
		}

		void logMessage(int level, std::string message) {
			if (level_ <= level) {
				write(message);
			}

			if (nextLogger_) {
				nextLogger_->logMessage(level, message);
			}
		}

	private:
		virtual void write(std::string& message) = 0;

	protected:
		int				level_;
		AbstractLogger* nextLogger_;
	};

	class ConsoleLogger : public AbstractLogger
	{
	public:
		ConsoleLogger() {
			level_ = INFO;
			nextLogger_ = nullptr;
		}

		~ConsoleLogger() override {

		}

	private:
		void write(std::string& message) {
			std::cout << "INFO: " << message << std::endl;
		}
	};

	class DebugLogger : public AbstractLogger
	{
	public:
		DebugLogger() {
			level_ = DEBUG;
			nextLogger_ = nullptr;
		}

		~DebugLogger() override {

		}

	private:
		void write(std::string& message) {
			std::cout << "DEBUG: " << message << std::endl;
		}
	};

	class ErrorLogger : public AbstractLogger
	{
	public:
		ErrorLogger() {
			level_ = ERROR;
			nextLogger_ = nullptr;
		}

		~ErrorLogger() override {

		}

	private:
		void write(std::string& message) {
			std::cout << "ERROR: " << message << std::endl;
		}
	};

	namespace ChainPatternDemo
	{
		void test()
		{
			std::cout << "\n\n chain of responsibility pattern." << std::endl;

			AbstractLogger* infoLogger = new ConsoleLogger();
			AbstractLogger* debugLogger = new DebugLogger();
			AbstractLogger* errorLogger = new ErrorLogger();

			errorLogger->setNextLogger(debugLogger);
			debugLogger->setNextLogger(infoLogger);

			errorLogger->logMessage(ERROR, "failed to find Mr.Cai");
			errorLogger->logMessage(DEBUG, "failed to find Mr.Xie");
			errorLogger->logMessage(INFO,  "failed to find Mr.Wang");

			Safe_Delete(infoLogger);
			Safe_Delete(debugLogger);
			Safe_Delete(errorLogger);
		}
	}
}

#endif //  __Chain_Logger_Inc_H__
