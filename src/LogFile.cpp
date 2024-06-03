#include "LogFile.h"
#include "ConfFile.h"

static constexpr std::uintmax_t MAX_LOG_FILE_SIZE = 2.0 * 1024 * 1024 * 1024;

extern std::string LOG_FILE_PATH;

LogFile*       LogFile::m_log_file_ = nullptr;
std::once_flag LogFile::once_flag_;

LogFile* LogFile::getInstance()
{
    std::call_once(once_flag_, initInstance);
    return m_log_file_;
}

void LogFile::initInstance()
{
    m_log_file_ = new LogFile();
}

void LogFile::destroyInstance()
{
    delete m_log_file_;
}

bool LogFile::writeLog(const Buffer& logs)
{
    writeLogToFile(logs);

    if (currentLogFileLarge() || currentDateJudge())
    {
        createNewLogFile();
    }

    return true;
}

LogFile::LogFile()
    : today_log_file_count_{0},
      current_date_{std::move(common::getCurrentDate())}
{
    ConfFile conf_file(LOG_FILE_PATH);
    log_directory_ = std::filesystem::current_path().append(conf_file.getParameterString("log", "directory", "../log"));
    log_path_      = getLogFileName();
    log_file_      = getLogFile(log_path_);
}

LogFile::~LogFile()
{
    log_file_.close();
}

inline bool LogFile::currentLogFileLarge() const
{
    return file_size(log_path_) >= MAX_LOG_FILE_SIZE;
}

inline bool LogFile::currentDateJudge() const
{
    return common::getCurrentDate() > current_date_;
}

void LogFile::writeLogToFile(const Buffer& logs)
{
    log_file_.write(logs.read(), logs.size());
    log_file_.flush();
}

void LogFile::createNewLogFile()
{
    // rename old log file
    t_log_path old_log_file_name;
    do
    {
        ++today_log_file_count_;
        old_log_file_name = getLogFileName(false);
    } while (exists(old_log_file_name));
    log_file_.close();
    fs::rename(log_path_, old_log_file_name);

    if (currentDateJudge())
    {
        today_log_file_count_ = 0;
        current_date_         = common::getCurrentDate();
    }
    // The log file count of the newest log file is always 0
    log_file_ = std::move(getLogFile(getLogFileName()));
}

std::string LogFile::getLogFileName(const bool is_newest) const
{
    if (!exists(log_directory_))
    {
        create_directories(log_directory_);
    }

    std::string log_file_name;
    if (is_newest)
    {
        log_file_name = getLogFileName(log_directory_, current_date_, 0);
    }
    else
    {
        log_file_name = getLogFileName(log_directory_, current_date_, today_log_file_count_);
    }

    return log_file_name;
}

LogFile::t_log_file LogFile::getLogFile(const t_log_path& log_path)
{
    t_log_file log_file(log_path, std::ios::out | std::ios::app);

    if (!log_file.is_open())
    {
        // TODO: How to deal with a situation where a logFile fails to be created?
    }

    return log_file;
}

std::string LogFile::getLogFileName(const std::string& log_directory,
                                    const std::string& current_date,
                                    const std::size_t  today_log_file_count)
{
    // ./test_20240401_0.log
    return log_directory + "/" + common::getCurrentProcessName() + "_" + current_date + "_" +
           std::to_string(today_log_file_count) + ".log";
}
