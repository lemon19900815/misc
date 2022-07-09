// Copyright (c), lemon19900815.
// All rights reserved.
// mini dump file generator for Windows, 
// use virtual studio to open the *.dmp file, locate the dump position.
// dependency: c++11 needed on Windows platform.

#ifndef __DUMP_HELPER_H__
#define __DUMP_HELPER_H__

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>

#ifdef _WIN32 || WIN32
#include <Windows.h>
#include <ImageHlp.h>
#include <atlstr.h>

#pragma comment(lib, "DbgHelp.lib")

static std::string Timestamp2Str(time_t ts) {
    struct tm* t = std::localtime(&ts);
    if (!t) return "";

    char buff[64];
    snprintf(buff, sizeof(buff),
        "%d-%02d-%02d-%02d-%02d-%02d",
        t->tm_year + 1900, t->tm_mon + 1,
        t->tm_mday, t->tm_hour,
        t->tm_min, t->tm_sec);

    return buff;
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
    size_t origsize = orig.length() + 1;
    size_t convertedChars = 0;
    wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}

long __stdcall callback(_EXCEPTION_POINTERS* excp) {
    auto now_ts = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    auto dump_time = Timestamp2Str(now_ts);

#ifdef UNICODE
    auto dump_file = stringToLPCWSTR(dump_time + ".dmp");

    HANDLE lhDumpFile = CreateFile(dump_file,
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    free((void*)dump_file);
#else
    HANDLE lhDumpFile = CreateFile((dump_time + ".dmp").c_str(),
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#endif

    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
    loExceptionInfo.ExceptionPointers = excp;
    loExceptionInfo.ThreadId = GetCurrentThreadId();
    loExceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(),
        GetCurrentProcessId(), lhDumpFile,
        MiniDumpNormal, &loExceptionInfo, NULL, NULL);

    CloseHandle(lhDumpFile);

    return EXCEPTION_EXECUTE_HANDLER;
}

// windows平台宕机生成mini.dmp文件，使用vs打开dmp文件辅助定位问题
class DumpHelper {
public:
    DumpHelper() {
        SetUnhandledExceptionFilter(callback);
    }

    ~DumpHelper() {
    }
};
#else
// 其他平台（如：Linux）可以通过gdb来定位
class DumpHelper {
public:
    DumpHelper() {
    }

    ~DumpHelper() {
    }
};
#endif // _WIN32

#endif // __DUMP_HELPER_H__
