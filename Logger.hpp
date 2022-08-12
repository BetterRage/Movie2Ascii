#pragma once
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
	Logger();
	/**
	 * @brief Logger using spdlog
	 *
	 * @param name name of the logger (appears in messages)
	 * @param tofile if logging to a file is required
	 */
	Logger(std::string name, bool tofile = true,std::string logfilepath = "");

	template <typename... Args>
	void logDebug(Args... args);

	template <typename... Args>
	void logWarning(Args... args);

	template <typename... Args>
	void logError(Args... args);

	template <typename... Args>
	void logInfo(Args... args);

private:
	std::shared_ptr<spdlog::logger> mLogger;
};

class Sinks 
{
public:
	static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> getConsoleSink();

	static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> getFileSink(std::string path = "");

private:
	static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
	static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;
};