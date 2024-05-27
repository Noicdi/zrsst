#ifndef ZRSST_INCLUDE_LOG_FILE_H_
#define ZRSST_INCLUDE_LOG_FILE_H_

#include <filesystem>
#include <fstream>
#include <mutex>

#include "Buffer.h"
#include "Common.h"

namespace fs = std::filesystem;

class LogFile
{
public:
    typedef fs::path      t_log_path;
    typedef std::ofstream t_log_file;

public:
    static LogFile* getInstance();

    static void initInstance();

    static void destroyInstance();

    bool writeLog(const Buffer& logs);

private:
    LogFile();

    ~LogFile();

    LogFile(const LogFile&) = delete;

    LogFile(LogFile&&) = delete;

    LogFile& operator=(const LogFile&) = delete;

    LogFile& operator=(LogFile&&) = delete;

    bool currentLogFileLarge() const;

    bool currentDateJudge() const;

    void writeLogToFile(const Buffer& logs);

    void createNewLogFile();

    static t_log_file getLogFile(const t_log_path& log_path);

    static std::string getLogFileName(const std::string& log_directory,
                                      const std::string& current_date,
                                      std::size_t        today_log_file_count
    );

private:
    static LogFile*       m_log_file_;
    static std::once_flag once_flag_;

    std::size_t today_log_file_count_;
    std::string current_date_;
    t_log_path  log_directory_;
    t_log_path  log_path_;
    t_log_file  log_file_;
};

#endif // ZRSST_INCLUDE_LOG_FILE_H_
