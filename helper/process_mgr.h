#ifndef __PROCESS_MGR_H__
#define __PROCESS_MGR_H__

#include <filesystem> // c++17
namespace fs = std::filesystem;

// 进程管理控制块（接口部分）
namespace process
{

/*
* 函数名称：start
* 函数功能：启动进程
* 输入参数：path_name 【完整路径的】程序名
*           args       启动程序的命令行参数
* 输出参数：无
* 返 回 值：返回启动的进程id
*/
int start(const std::string& path_name, const std::string& args);

/*
* 函数名称：stop
* 函数功能：关闭进程
* 输入参数：path_name 【完整路径的】程序名
* 输出参数：无
* 返 回 值：成功返回true，否则返回false
*/
bool stop(const std::string& path_name);

/*
* 函数名称：is_running
* 函数功能：检测进程是否处于运行中
* 输入参数：path_name 【完整路径的】程序名
* 输出参数：无
* 返 回 值：成功返回true，否则返回false
*/
bool is_running(const std::string& path_name);

}

// 获取绝对路径
static std::string get_abspath(const std::string& path_name)
{
    std::error_code ec;
    return fs::canonical(fs::path{ path_name }, ec).string();
}

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <tlhelp32.h>//声明快照函数的头文件

// 启动进程
int process::start(const std::string& path_name, const std::string& args)
{
    auto pname = fs::path(get_abspath(path_name));

    if (!fs::exists(pname))
    {
        std::cerr << "ERROR: not exist process: " << path_name << std::endl;
        return -1;
    }

    using namespace std::string_literals;

    STARTUPINFO start_info;
    PROCESS_INFORMATION process_info;
    memset(&start_info, 0, sizeof(STARTUPINFO));
    start_info.cb = sizeof(STARTUPINFO);
    start_info.dwFlags = STARTF_USESHOWWINDOW;
    start_info.wShowWindow = SW_SHOW;

    //CreateProcess(_Convert_Source_to_wide(process_with_param).c_str(),
    //    const_cast<wchar_t*>(L""), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
    //    NULL, NULL, &start_info, &process_info);

    // 转换为宽字符
    auto ws_param = fs::path(" "s + args).native();

    CreateProcess(
        pname.native().c_str(), ws_param.data(),
        NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
        NULL, NULL, &start_info, &process_info);

    //handle_ = process_info.hProcess;
    //process_ = p.filename().native();

    //LOG(INFO) << "2cur path = " << std::filesystem::current_path();

    //WinExec(process_with_param.c_str(), 0);

    return process_info.dwProcessId;
}

bool query_win_process(const std::string& process,
    std::function<void(PROCESSENTRY32& proc)>&& visitor = nullptr)
{
    auto ws_process = std::filesystem::path(process).filename().native();

    auto to_lower = [](std::wstring src)->std::wstring
    {
        std::transform(src.begin(), src.end(), src.begin(), ::tolower);
        return src;
    };

    ws_process = to_lower(ws_process);

    // 获取系统中所有进程的快照。
    auto snap_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // 在使用结构之前设置结构的大小。
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // 获取第一个进程的信息
    // 如果不成功则退出
    if (!Process32First(snap_handle, &pe32))
    {
        CloseHandle(snap_handle); // clean the snapshot object
        return false;
    }

    bool is_finded = false;

    //现在遍历进程的快照，并且依次显示每个进程的信息
    do
    {
        //std::cout << std::filesystem::path(pe32.szExeFile).string() << std::endl;
        // Retrieve the priority class.
        if (ws_process == to_lower(std::wstring(pe32.szExeFile)))
        {
            if (visitor)
            {
                visitor(pe32);
            }
            is_finded = true;
            break;
        }
    } while (Process32Next(snap_handle, &pe32));

    CloseHandle(snap_handle);
    return is_finded;
}

// 关闭进程
bool process::stop(const std::string& path_name)
{
    return query_win_process(path_name, [&path_name](PROCESSENTRY32& proc)
    {
        auto proc_handle = OpenProcess(PROCESS_ALL_ACCESS,
                                       FALSE, proc.th32ProcessID);
        if (proc_handle)
        {
            TerminateProcess(proc_handle, 0);
        }
        else
        {
            std::cerr << "stop process: " << path_name << " failed." << std::endl;
        }
    });
}

// 检测进程是否处于运行中
bool process::is_running(const std::string& path_name)
{
    return !path_name.empty() && query_win_process(path_name);
}

#else // linux
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 根据进程名（全路径或者相对路径）获取进程id
int get_pid_by_name(const char* pname)
{
    if (!pname) {
        perror("pname is nullptr.\n");
        return -1;
    }

    DIR* dir = NULL;
    struct dirent* entry = NULL;
    char exe[512] = { 0 };
    char path[512] = { 0 };
    int pid = -1;

    dir = opendir("/proc");
    if (!dir) {
        perror("can't open dir /proc\n");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }

        if (entry->d_type != DT_DIR) {
            continue;
        }

        sprintf(exe, "/proc/%s/exe", entry->d_name);

        if (readlink(exe, path, sizeof(path)) == -1) {
            continue;
        }

        if (strstr(path, pname) != NULL) {
            pid = atoi(entry->d_name);
            break;
        }
    }

    closedir(dir);

    return pid;
}

// 启动进程
int process::start(const std::string& path_name, const std::string& args)
{
    auto pname = fs::path(get_abspath(path_name));

    if (!fs::exists(pname))
    {
        std::cerr << "ERROR: not exist process: " << pname << std::endl;
        return -1;
    }

    std::cout << "pname = " << pname << std::endl;

    // 创建子进程去启动第三方程序
    auto pid = fork();
    if (pid < 0)
    {
        std::cerr << "ERROR: fork failed." << std::endl;
        return -1;
    }

    if (pid == 0)
    {
        auto proc = pname.string();
        // child process space.
        char* proc_args[] = { proc.data(),
                              (char*)args.data(), nullptr };

        //execv(proc_args[0], proc_args);
        //execv(pname.parent_path().string().data(), proc_args);
        execl(proc.data(), proc.data(), (char*)args.data(), nullptr);
        exit(0); // 子进程执行失败直接退出
    }

    // pid > 0: 父进程空间，pid为子进程id
    return pid;
}

// 关闭进程
bool process::stop(const std::string& path_name)
{
    auto pname = get_abspath(path_name);
    auto pid = get_pid_by_name(pname.c_str());

    return pid > 0 && kill(pid, SIGTERM) == 0;
}

// 检测进程是否处于运行中
bool process::is_running(const std::string& path_name)
{
    auto pname = get_abspath(path_name);
    auto pid = get_pid_by_name(pname.c_str());

    return pid > 0;
}

#endif // linux

#endif // __PROCESS_MGR_H__
