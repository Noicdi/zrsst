#ifndef ZRSST_INCLUDE_COMMON_H_
#define ZRSST_INCLUDE_COMMON_H_

#include <string>

namespace common
{
// return: 19700101 00:00:00
std::string getCurrentDateTime();

// return: 19700101
std::string getCurrentDate();

// return: 00:00:00
std::string getCurrentTime();

std::string getCurrentProcessName();

void stringTrim(std::string&);

void stringToLower(std::string&);

void stringToUpper(std::string&);
} // namespace common

#endif // ZRSST_INCLUDE_COMMON_H_
ZRSST_INCLUDE_COMMON_H_
