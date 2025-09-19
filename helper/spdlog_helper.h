#ifndef SPDLOG_HELPER_H
#define SPDLOG_HELPER_H

#include <iomanip>

// https://github.com/gabime/spdlog
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

// https://github.com/guangie88/spdlog_setup
#include "spdlog_setup/conf.h"

class spdlog_helper
{
public:
    spdlog_helper(
        const std::string logger_name,
        bool log_to_stdout = false,
        std::string log_dir = "log")
    {
        std::vector<spdlog::sink_ptr> sinks;
        using namespace spdlog::sinks;

        if (log_to_stdout)
        {
            sinks.emplace_back(std::make_shared<stdout_color_sink_mt>());
            sinks.back()->set_level(spdlog::level::info);
        }

        if (!log_dir.empty() &&
            (log_dir.back() != '\\' || log_dir.back() != '/'))
        {
            log_dir.append("/");
        }

        // create a new log file on process startup.
        auto date_time = get_time_fmt("%Y%m%d-%H%M%S");
        auto name = fmt::format("{}{}.txt", log_dir, date_time);

        constexpr auto kMaxLogFileSize = 1024 * 1024 * 100;
        constexpr auto kMaxFiles = 200000; // 最大文件数量200000

        sinks.push_back(std::make_shared<rotating_file_sink_mt>(
                            name, kMaxLogFileSize, kMaxFiles));
        sinks.back()->set_level(spdlog::level::info);

        std::shared_ptr<spdlog::logger> combined_logger;

        combined_logger = std::make_shared<spdlog::logger>(
                              logger_name, begin(sinks), end(sinks));
        combined_logger->set_level(spdlog::level::info);
        combined_logger->flush_on(spdlog::level::err);

        spdlog::set_default_logger(combined_logger);

        // log pattern: enable function name.
        // reference to: pattern_formatter-inl.h:pattern_formatter::handle_flag_
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%# %!] %v");

        // Flush all *registered* loggers using a worker thread every 3 seconds.
        // note: registered loggers *must* be thread safe for this to work correctly!
        spdlog::flush_every(std::chrono::seconds(3));
    }
    
    spdlog_helper(const std::string toml_file)
    {
        try
        {
            // set global log level. Not set, SPDLOG_DEBUG will not log to file.
            spdlog::set_level(spdlog::level::debug);

            // initialize service log system.
            spdlog_setup::from_file(toml_file);

            // Flush all *registered* loggers using a worker thread every 3 seconds.
            // note: registered loggers *must* be thread safe for this to work correctly!
            spdlog::flush_every(std::chrono::seconds(3));
        }
        catch (std::exception& ex)
        {
            SPDLOG_ERROR("spdlog_helper init failed: {}", ex.what());
        }
    }

private:
    std::string get_time_fmt(const std::string fmt = "%Y-%m-%d %H:%M:%S")
    {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::system_clock::to_time_t(now);

        auto stm = std::localtime(&timestamp);

        std::ostringstream ostr;
        ostr << std::put_time(stm, fmt.c_str());

        return ostr.str();
    }
};

#endif // SPDLOG_HELPER_H
