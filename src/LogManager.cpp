#include <unordered_map>

#include "LogManager.h"

static constexpr std::size_t    MAX_WRITE_LOG_FILE_WAIT_SECOND = 3;
static constexpr std::uintmax_t MAX_LOG_BUFFER_SIZE            = 4.0 * 1024;

LogManager* LogManager::getInstance()
{
    static LogManager m_log_manager_(INFO);

    return &m_log_manager_;
}

void LogManager::writeLog(const std::string& log, const logLevel log_level)
{
    if (log.empty())
    {
        return;
    }

    // 19700101 00:00:00 INFO  -- This is a log
    const std::string log_info = std::move(common::getCurrentDateTime() + logLevelToString(log_level));

    std::lock_guard write_log_lock(log_mutex_);

    if (current_buffer_.get()->canWrite(log_info.size() + log.size() + 1))
    {
        current_buffer_.get()->append(log_info);
        current_buffer_.get()->append(log);
        current_buffer_.get()->append("\n");
    }
    else
    {
        switchCurrentBuffer();

        current_buffer_.get()->append(log_info);
        current_buffer_.get()->append(log);
        current_buffer_.get()->append("\n");

        write_log_variable_.notify_one();
    }
}

LogManager::logLevel LogManager::getLogLevel() const
{
    return log_level_;
}

void LogManager::setLogLevel(logLevel log_level)
{
    log_level_ = log_level;
}

LogManager::LogManager(const logLevel log_level)
    : is_run_{true},
      log_level_{log_level},
      current_buffer_{std::make_unique<t_log_buffer>(MAX_LOG_BUFFER_SIZE)},
      write_log_thread_{&LogManager::run, this}
{}

LogManager::~LogManager()
{
    is_run_ = false;
    write_log_thread_.join();
    LogFile::getInstance()->writeLog(*current_buffer_);
    LogFile::destroyInstance();
}

void LogManager::run()
{
    t_log_buffer_vector write_buffer_vector;

    while (is_run_)
    {
        {
            std::unique_lock<std::mutex> lock(log_mutex_);
            write_log_variable_.wait_for(lock,
                                         std::chrono::seconds(MAX_WRITE_LOG_FILE_WAIT_SECOND),
                                         [&]
                                         {
                                             return !write_buffer_vector_.empty() || !current_buffer_.get()->empty();
                                         }
                                        );

            if (write_buffer_vector_.empty())
            {
                switchCurrentBuffer();
            }

            write_buffer_vector.swap(write_buffer_vector_);
        }

        for (auto& write_buffer : write_buffer_vector)
        {
            LogFile::getInstance()->writeLog(*write_buffer);
            write_buffer->clear();
            empty_buffer_vector_.emplace_back(std::move(write_buffer));
        }

        write_buffer_vector.clear();
    }
}

void LogManager::switchCurrentBuffer()
{
    write_buffer_vector_.emplace_back(std::move(current_buffer_));
    if (!empty_buffer_vector_.empty())
    {
        current_buffer_ = std::move(empty_buffer_vector_.back());
        empty_buffer_vector_.pop_back();
    }
    else
    {
        current_buffer_ = std::make_unique<t_log_buffer>(MAX_LOG_BUFFER_SIZE);
    }
}

inline LogManager::t_log_level_string LogManager::logLevelToString(const logLevel log_level)
{
    // PERF: How to enhance the performance of logLevelToString() ?
    static const std::unordered_map<logLevel, t_log_level_string> level_map = {
        {DEBUG, " DEBUG -- "},
        {INFO, " INFO  -- "},
        {WARN, " WARN  -- "},
        {ERROR, " ERROR -- "},
        {FATAL, " FATAL -- "}
    };

    return level_map.find(log_level)->second;
}
