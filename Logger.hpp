#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
	Logger(std::string name, spdlog::level::level_enum level = spdlog::level::info)
	{
		initLogger(name, {getConsoleSink()}, level);
	}

	Logger(std::string name, std::string logfilepath, spdlog::level::level_enum level = spdlog::level::info)
	{
		initLogger(name, {getFileSink(logfilepath), getConsoleSink()}, level);
	}

	template <typename... Args>
	void logDebug(Args... args) { mLogger->debug(args...); }

	template <typename... Args>
	void logWarning(Args... args) { mLogger->warn(args...); }

	template <typename... Args>
	void logError(Args... args) { mLogger->error(args...); }

	template <typename... Args>
	void logInfo(Args... args) { mLogger->info(args...); }

private:
	const std::string pattern = "[%H:%M:%S] %-11n %^-%L-%$ > %v";
	std::shared_ptr<spdlog::logger> mLogger;

	void initLogger(std::string name, spdlog::sinks_init_list sinks, spdlog::level::level_enum level)
	{
		mLogger = std::make_shared<spdlog::logger>(name, sinks);
		mLogger->set_level(level);
		mLogger->set_pattern(pattern);
		mLogger->flush_on(spdlog::level::info);
	}

	static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> &getConsoleSink()
	{
		static auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		return sink;
	}

	static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> &getFileSink(std::string path)
	{
		static auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path);
		return sink;
	}
};
#endif // __LOGGER_H__