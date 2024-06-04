#ifndef CONFFILE_H
#define CONFFILE_H

#include <fstream>
#include <string>
#include <unordered_map>

class ConfFile
{
  public:
    typedef std::ifstream                                          t_conf_file;
    typedef std::unordered_map<std::string, std::string>           t_conf_file_parameter;
    typedef std::unordered_map<std::string, t_conf_file_parameter> t_conf_file_section;

  public:
    explicit ConfFile(const std::string& conf_file_path);

    ~ConfFile();

    std::string getParameterString(const std::string& section,
                                   const std::string& parameter_key,
                                   const std::string& parameter_value_default);

    int getParameterInt(const std::string& section, const std::string& parameter_key, int parameter_value_default);

    bool getParameterBool(const std::string& section, const std::string& parameter_key, bool parameter_value_default);

  private:
    void readParameter();

    static bool isSection(const std::string& str);

    static bool isComment(const std::string& str);

    static void eraseComment(std::string& str);

  private:
    t_conf_file         conf_file_;
    t_conf_file_section conf_file_section_;
};

#endif // CONFFILE_H
