#include <iostream>

#include "Common.h"

int main()
{
    std::cout << "current date time: " << common::getCurrentDateTime() << std::endl;
    std::cout << "current date: " << common::getCurrentDate() << std::endl;
    std::cout << "current time: " << common::getCurrentTime() << std::endl;

    return 0;
}