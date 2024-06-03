#include <algorithm>
#include <chrono>
#include <filesystem>

#include "Common.h"


namespace common
{
std::string getCurrentDateTime()
{
    const auto now    = std::chrono::system_clock::now();
    const auto now_tt = std::chrono::system_clock::to_time_t(now);

    std::stringstream current_date_time;
    current_date_time << std::put_time(std::localtime(&now_tt), "%Y%m%d %T");

    return current_date_time.str();
}

std::string getCurrentDate()
{
    auto current_date_time = getCurrentDateTime();

    return current_date_time.substr(0, current_date_time.find(' '));
}

std::string getCurrentTime()
{
    const auto current_date_time = getCurrentDateTime();

    return current_date_time.substr(current_date_time.find(' ') + 1);
}

std::string getCurrentProcessName()
{
    static const std::filesystem::path exe_path = std::filesystem::read_symlink("/proc/self/exe");

    return exe_path.filename().string();
}

void stringTrim(std::string& str)
{
    if (str.empty())
    {
        return;
    }

    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

void stringToLower(std::string& str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(), [](const unsigned char c) { return std::tolower(c); });
}

void stringToUpper(std::string& str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(), [](const unsigned char c) { return std::toupper(c); });
}
} // namespace common
