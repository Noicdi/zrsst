add_executable(test-log
        ../src/Common.cpp
        ../src/Buffer.cpp
        ../src/LogFile.cpp
        ../src/LogManager.cpp
        ../src/ConfFile.cpp
        TestLog.cpp
)

add_executable(test-common
        ../src/Common.cpp
        TestCommon.cpp
)
