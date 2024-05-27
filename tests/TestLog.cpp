#include <unistd.h>
#include <thread>
#include <chrono>

#include "Log.h"

using namespace std::chrono_literals;

int main()
{
    logDebug("1 This is a debug log");
    logDebug("1 This is a debug log %d", 1);

    logInfo("2 This is a info log");
    logInfo("2 This is a info log %d", 2);

    logWarn("3 This is a warn log");
    logWarn("3 This is a warn log %d", 10.5);

    logError("4 This is a error log");
    logError("4 This is a error log %s", "test");

    logInfo("5 This is a info log ddd");
    logInfo("5 This is a info log %dddd", 2);

    logError("6 This is a error log ccc");
    logError("6 This is a error log %sccc", "test");

    // std::this_thread::sleep_for(7s);
    return 0;
}
