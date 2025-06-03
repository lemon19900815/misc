// Copyright (c), buerjia.
// All rights reserved.

#pragma once

#include <tuple>
#include <cstdint>
#include <algorithm>

#include <Windows.h>

namespace monitor
{
    // cpu total usage percent.
    double get_cpu_usage();

    // mem usage percent.
    double get_mem_usage();

    // network io sent&recv bytes.
    void get_io_bytes(uint64_t& sent, uint64_t& recv);
} // namespace monitor

namespace monitor {
namespace details {

struct cpu_time {
    double user = 0;
    double system = 0;
    double idle = 0;
};

static cpu_time get_cpu_time() {
    // psutil_cpu_times
    // https://github.com/giampaolo/psutil/blob/master/psutil/arch/windows/cpu.c#L48
    double idle, kernel, user, system;
    FILETIME idle_time, kernel_time, user_time;

    if (!GetSystemTimes(&idle_time, &kernel_time, &user_time)) {
        return {};
    }

    constexpr auto LO_T = 1e-7;
    constexpr auto HI_T = 429.4967296;

    idle = (double)((HI_T * idle_time.dwHighDateTime) + \
        (LO_T * idle_time.dwLowDateTime));
    user = (double)((HI_T * user_time.dwHighDateTime) + \
        (LO_T * user_time.dwLowDateTime));
    kernel = (double)((HI_T * kernel_time.dwHighDateTime) + \
        (LO_T * kernel_time.dwLowDateTime));

    // Kernel time includes idle time.
    // We return only busy kernel time subtracting idle time from
    // kernel time.
    system = (kernel - idle);

    cpu_time info;
    info.idle = idle;
    info.system = system;
    info.user = user;

    return info;
}

static cpu_time delta_cpu_time(const cpu_time& t1, const cpu_time& t2) {
    cpu_time t;
    t.idle = std::max<double>(0, t2.idle - t1.idle);
    t.system = std::max<double>(0, t2.system - t1.system);
    t.user = std::max<double>(0, t2.user - t1.user);
    return t;
}

static double get_busy_time(const cpu_time& cpu_tm) {
    return cpu_tm.system + cpu_tm.user;
}

static double get_total_time(const cpu_time& cpu_tm) {
    return cpu_tm.system + cpu_tm.user + cpu_tm.idle;
}

} // namespace details

static double get_cpu_usage() {
    static auto last_cpu_tm = details::get_cpu_time();

    auto cur_cpu_tm = details::get_cpu_time();
    auto delta_cpu_tm = details::delta_cpu_time(last_cpu_tm, cur_cpu_tm);

    auto total_time = details::get_total_time(delta_cpu_tm);
    auto busy_time = details::get_busy_time(delta_cpu_tm);

    last_cpu_tm = cur_cpu_tm;

    if (total_time == 0.0)
        return 0.0;

    return busy_time / total_time * 100;
}

static double get_mem_usage() {
    MEMORYSTATUS status;
    ::GlobalMemoryStatus(&status);
    return (status.dwTotalPhys - status.dwAvailPhys) * 100.0 / status.dwTotalPhys;
}

static void get_io_bytes(uint64_t& sent, uint64_t& recv) {
    IO_COUNTERS io_counter;
    if (GetProcessIoCounters(GetCurrentProcess(), &io_counter)) {
        sent = io_counter.WriteTransferCount;
        recv = io_counter.ReadTransferCount;
    }
}

} // namespace monitor
