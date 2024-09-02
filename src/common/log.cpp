/********************************************
 * Filename: log.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include "common/log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define LOG_FILE_SIZE 10 * 1024 * 1024   // 10MB
#define LOG_FILE_NUM 3

namespace AVCPP_EXAMPLE
{

    avcppExampleLog::avcppExampleLog()
    {
        m_logger_name = "avcpp_exmaple";
        m_logger_level = AVCPP_EXAMPLE_LOG_LEVEL_INFO;
        m_logger_file_name = "/tmp/log.txt";
        // init log rotate config
        m_logger_file_size = LOG_FILE_SIZE;
        m_logger_file_num = LOG_FILE_NUM;
    }

    avcppExampleLog& avcppExampleLog::Instance()
    {
        static avcppExampleLog log;
        return log;
    }

    void avcppExampleLog::initAvcppExampleLog(std::string file_name, int log_level)
    {
        // set log level
        if (log_level != AVCPP_EXAMPLE_LOG_LEVEL_TRACE && log_level != AVCPP_EXAMPLE_LOG_LEVEL_DEBUG && 
            log_level != AVCPP_EXAMPLE_LOG_LEVEL_INFO  && log_level != AVCPP_EXAMPLE_LOG_LEVEL_WARN  && 
            log_level != AVCPP_EXAMPLE_LOG_LEVEL_ERROR && log_level != AVCPP_EXAMPLE_LOG_LEVEL_FATAL && 
            log_level != AVCPP_EXAMPLE_LOG_LEVEL_OFF)
        {
            std::cout << "get invalid log level " << log_level << ", will use INFO as default" << std::endl;
            m_logger_level = AVCPP_EXAMPLE_LOG_LEVEL_INFO;
        }
        else
            m_logger_level = log_level;

        // set log rotate
        if (!file_name.empty())
            m_logger_file_name = file_name;

        std::cout << "log level: " << m_logger_level << std::endl;
        std::cout << "logger name: " << m_logger_name << std::endl;
        std::cout << "log file path: " << m_logger_file_name << std::endl;

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(m_logger_file_name, 
            m_logger_file_size, m_logger_file_num);
        m_logger.reset();
        m_logger = std::unique_ptr<spdlog::logger>(new spdlog::logger(m_logger_name, {console_sink, file_sink}));
        spdlog::register_logger(m_logger);
        // set log pattern
        m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e %z] [%n] [%^---%L---%$] [thread %t] [%g:%# %!] %v");
        spdlog::set_default_logger(spdlog::get(m_logger_name));
        spdlog::flush_on(spdlog::level::debug);
        spdlog::set_level(static_cast<spdlog::level::level_enum>(m_logger_level));
    }

    void avcppExampleLog::stopAvcppExampleLog()
    {
        spdlog::shutdown();
        m_logger.reset();
    }

    void avcppExampleLog::setLevel(int level)
    {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    }

} // namespace AVCPP_EXAMPLE