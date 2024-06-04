#include "ConfFile.h"

#include "Common.h"

#include <filesystem>

static const auto CONF_FILE_SECTION_DEFAULT = "default";

ConfFile::ConfFile(const std::string& conf_file_path)
{
    conf_file_section_.emplace(CONF_FILE_SECTION_DEFAULT, t_conf_file_parameter{});

    if (!std::filesystem::exists(conf_file_path))
    {
        return;
    }
    conf_file_.open(conf_file_path, std::ios::in);

    readParameter();
}

ConfFile::~ConfFile()
{
    conf_file_.close();
}

void ConfFile::readParameter()
{
    auto section_iter = conf_file_section_.find(CONF_FILE_SECTION_DEFAULT);

    for (std::string line; std::getline(conf_file_, line);)
    {
        eraseComment(line);
        common::stringTrim(line);
        if (line.empty() || isComment(line))
        {
            continue;
        }

        if (isSection(line)) // conf file section
        {
            auto section = line.substr(1, line.length() - 2);
            common::stringToLower(section);

            section_iter = conf_file_section_.find(section);
            if (section_iter == conf_file_section_.end())
            {
                conf_file_section_.emplace(section, t_conf_file_parameter{});
                section_iter = conf_file_section_.find(section);
            }
        }
        else // conf file parameter
        {
            const auto separator       = line.find('=');
            auto       parameter_key   = line.substr(0, separator);
            auto       parameter_value = line.substr(separator + 1, line.length() - separator - 1);
            common::stringTrim(parameter_key);
            common::stringTrim(parameter_value);
            common::stringToLower(parameter_key);
            common::stringToLower(parameter_value);

            section_iter->second.emplace(parameter_key, parameter_value);
        }
    }
}

std::string ConfFile::getParameterString(const std::string& section,
                                         const std::string& parameter_key,
                                         const std::string& parameter_value_default)
{
    const auto section_iter = conf_file_section_.find(section);
    if (section_iter == conf_file_section_.cend())
    {
        return parameter_value_default;
    }

    const auto parameter_iter = section_iter->second.find(parameter_key);
    if (parameter_iter == section_iter->second.cend())
    {
        return parameter_value_default;
    }

    return parameter_iter->second;
}

int ConfFile::getParameterInt(const std::string& section,
                              const std::string& parameter_key,
                              const int          parameter_value_default)
{
    int result = parameter_value_default;
    try
    {
        result = std::stoi(getParameterString(section, parameter_key, std::to_string(parameter_value_default)));
    }
    catch (std::invalid_argument)
    {
        return parameter_value_default;
    }
    catch (std::out_of_range)
    {
        return parameter_value_default;
    }

    return result;
}

bool ConfFile::getParameterBool(const std::string& section,
                                const std::string& parameter_key,
                                const bool         parameter_value_default)
{
    std::string result_string;
    if (parameter_value_default)
    {
        result_string = getParameterString(section, parameter_key, "true");
    }
    else
    {
        result_string = getParameterString(section, parameter_key, "false");
    }

    if (result_string == "false")
    {
        return false;
    }

    return true;
}

inline bool ConfFile::isSection(const std::string& str)
{
    if (str.empty())
    {
        return false;
    }

    return str[0] == '[' && str[str.length() - 1] == ']';
}

inline bool ConfFile::isComment(const std::string& str)
{
    if (str.empty())
    {
        return false;
    }

    return str[0] == '#';
}

inline void ConfFile::eraseComment(std::string& str)
{
    if (str.empty())
    {
        return;
    }

    if (const auto pos = str.find_first_of('#'); pos != std::string::npos)
    {
        str.erase(pos);
    }
}
