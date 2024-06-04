#ifndef LOG_H
#define LOG_H

#include "LogManager.h"

#include <sstream>

// public log interface

template<typename T, typename... Targs>
void logDebug(const char* format, T value, Targs... Fargs);

void logDebug(const char* format);

template<typename T, typename... Targs>
void logInfo(const char* format, T value, Targs... Fargs);

void logInfo(const char* format);

template<typename T, typename... Targs>
void logWarn(const char* format, T value, Targs... Fargs);

void logWarn(const char* format);

template<typename T, typename... Targs>
void logError(const char* format, T value, Targs... Fargs);

void logError(const char* format);

template<typename T, typename... Targs>
void logFatal(const char* format, T value, Targs... Fargs);

void logFatal(const char* format);

// private log function

template<typename T, typename... Targs>
static void formatToString(std::ostringstream& stream, const char* format, T value, Targs... Fargs);

template<typename T, typename... Targs>
static void log(LogManager::logLevel level, const char* format, T value, Targs... Fargs);

template<typename T, typename... Targs>
void logDebug(const char* format, T value, Targs... Fargs)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::DEBUG)
    {
        return;
    }

    log(LogManager::logLevel::DEBUG, format, value, Fargs...);
}

inline void logDebug(const char* format)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::DEBUG)
    {
        return;
    }

    LogManager::getInstance()->writeLog(format, LogManager::logLevel::DEBUG);
}

template<typename T, typename... Targs>
void logInfo(const char* format, T value, Targs... Fargs)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::INFO)
    {
        return;
    }

    log(LogManager::logLevel::INFO, format, value, Fargs...);
}

inline void logInfo(const char* format)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::INFO)
    {
        return;
    }

    LogManager::getInstance()->writeLog(format, LogManager::logLevel::INFO);
}

template<typename T, typename... Targs>
void logWarn(const char* format, T value, Targs... Fargs)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::WARN)
    {
        return;
    }

    log(LogManager::logLevel::WARN, format, value, Fargs...);
}

inline void logWarn(const char* format)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::WARN)
    {
        return;
    }

    LogManager::getInstance()->writeLog(format, LogManager::logLevel::WARN);
}

template<typename T, typename... Targs>
void logError(const char* format, T value, Targs... Fargs)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::ERROR)
    {
        return;
    }

    log(LogManager::logLevel::ERROR, format, value, Fargs...);
}

inline void logError(const char* format)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::ERROR)
    {
        return;
    }

    LogManager::getInstance()->writeLog(format, LogManager::logLevel::ERROR);
}

template<typename T, typename... Targs>
void logFatal(const char* format, T value, Targs... Fargs)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::FATAL)
    {
        return;
    }

    log(LogManager::logLevel::FATAL, format, value, Fargs...);
}

inline void logFatal(const char* format)
{
    if (LogManager::getInstance()->getLogLevel() > LogManager::logLevel::FATAL)
    {
        return;
    }

    LogManager::getInstance()->writeLog(format, LogManager::logLevel::FATAL);
}

static void formatToString(std::ostringstream& stream, const char* format)
{
    for (; *format != '\0'; ++format)
    {
        stream << *format;
    }
}

template<typename T, typename... Targs>
static void formatToString(std::ostringstream& stream, const char* format, T value, Targs... Fargs)
{
    for (; *format != '\0'; ++format)
    {
        if (*format == '%' && *(format + 1) != '%')
        {
            stream << value;
            formatToString(stream, format + 2, Fargs...);
            return;
        }

        stream << *format;
    }
}

template<typename T, typename... Targs>
static void log(LogManager::logLevel level, const char* format, T value, Targs... Fargs)
{
    std::ostringstream stream;
    formatToString(stream, format, value, Fargs...);

    LogManager::getInstance()->writeLog(stream.str(), level);
}

#endif // LOG_H
