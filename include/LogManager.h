#ifndef ZRSST_SRC_LOG_MANAGER_H_
#define ZRSST_SRC_LOG_MANAGER_H_

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "Buffer.h"
#include "LogFile.h"

class LogManager
{
public:
    typedef std::string                                t_log_level_string;
    typedef Buffer                                     t_log_buffer;
    typedef std::unique_ptr<t_log_buffer>              t_log_buffer_ptr;
    typedef std::vector<std::unique_ptr<t_log_buffer>> t_log_buffer_vector;

    enum logLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

public:
    static LogManager* getInstance();

    void writeLog(const std::string& log, logLevel log_level = INFO);

    [[nodiscard]] logLevel getLogLevel() const;

    void setLogLevel(logLevel log_level);

private:
    explicit LogManager(logLevel log_level = INFO);

    ~LogManager();

    LogManager(const LogManager&) = delete;

    LogManager(LogManager&&) = delete;

    LogManager& operator=(const LogManager&) = delete;

    LogManager& operator=(LogManager&&) = delete;

    void run();

    void switchCurrentBuffer();

    static t_log_level_string logLevelToString(LogManager::logLevel log_level = INFO);

private:
    bool                    is_run_;
    logLevel                log_level_;
    t_log_buffer_ptr        current_buffer_;
    t_log_buffer_vector     write_buffer_vector_;
    t_log_buffer_vector     empty_buffer_vector_;
    std::mutex              log_mutex_;
    std::condition_variable write_log_variable_;
    std::thread             write_log_thread_;
};

#endif // ZRSST_SRC_LOG_MANAGER_H_
