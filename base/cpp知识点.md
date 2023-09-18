# cpp知识点记录

cpp-记录开发过程中遇到的问题、原因、解决办法。

## 1. c++17

### 1.1 file_time_type filesystem::last_write_time：文件最后修改时间问题

- Windows：filetime 是以1601年1月1日开始的100纳秒为单位的时间，system time(UTC)获取的时间是以1970年1月1日为其实的，之间相差的秒数为：11644473600，这一点用当前的filetime时间减去当前的systemtime时间可以验证：

  ```c++
  auto sys_time = std::chrono::system_clock::now().time_since_epoch();
  auto file_time = std::filesystem::file_time_type::clock::now().time_since_epoch();
  auto elapse = std::chrono::duration_cast<std::chrono::seconds>(sys_time - file_time).count();
  
  elapse: 11644473600
  ```

- Linux：filetime 是以system time(UTC)的时间为准，但是实际在Ubuntu下，获取的file_time_type的当前时间是负数。

  *阅读源码也没有发现造成ubuntu下该时间为负数的原因；最开始以为是_GLIBCXX_USE_ST_MTIM使用的结构体不一样导致，但实际测试发现这2个结构体的值是一样的；准备下载gcc的源码，再深入追踪一下--见下节。*

  ```c++
    inline std::chrono::system_clock::time_point
    file_time(const stat_type& st, std::error_code& ec) noexcept
    {
      using namespace std::chrono;
  #ifdef _GLIBCXX_USE_ST_MTIM
      time_t s = st.st_mtim.tv_sec;
      nanoseconds ns{st.st_mtim.tv_nsec};
  #else
      time_t s = st.st_mtime;
      nanoseconds ns{};
  #endif
  
      // FIXME
      // There are possible timespec values which will overflow
      // chrono::system_clock::time_point but would not overflow
      // __file_clock::time_point, due to its different epoch.
      //
      // By checking for overflow of the intermediate system_clock::duration
      // type, we report an error for values which are actually representable
      // in the file_time_type result type.
      //
      // Howard Hinnant's solution for this problem is to use
      // duration<__int128>{s} + ns, which doesn't overflow.
      // An alternative would be to do the epoch correction on s before
      // the addition, and then go straight to file_time_type instead of
      // going via chrono::system_clock::time_point.
      //
      // (This only applies to the C++17 Filesystem library, because for the
      // Filesystem TS we don't have a distinct __file_clock, we just use the
      // system clock for file timestamps).
      if (seconds{s} >= floor<seconds>(system_clock::duration::max()))
      {
          ec = std::make_error_code(std::errc::value_too_large); // EOVERFLOW
          return system_clock::time_point::min();
      }
      ec.clear();
      return system_clock::time_point{seconds{s} + ns};
    }
  
  fs::file_time_type
  fs::last_write_time(const path& p, error_code& ec) noexcept
  {
    return do_stat(p, ec, [&ec](const auto& st) { return file_time(st, ec); },
  		 file_time_type::min());
  }
  
  template<typename Accessor, typename T>
  inline T
  do_stat(const fs::path& p, std::error_code& ec, Accessor f, T deflt)
  {
      #ifdef _GLIBCXX_HAVE_SYS_STAT_H
      stat_type st;
      if (posix::stat(p.c_str(), &st))
      {
          ec.assign(errno, std::generic_category());
          return deflt;
      }
      ec.clear();
      return f(st);
      #else
      ec = std::make_error_code(std::errc::function_not_supported);
      return deflt;
      #endif
  }
  
  // 在测试代码中使用以下代码探测_GLIBCXX_USE_ST_MTIM宏是否生效
  #ifdef _GLIBCXX_USE_ST_MTIM
  	std::cout << "use _GLIBCXX_USE_ST_MTIM" << std::endl;
  #else
  	std::cout << "not use _GLIBCXX_USE_ST_MTIM" << std::endl;
  #endif
  ```

  *源码分析：*从源码层面来看，file_time_type和system time的时间相差`_S_epoch_diff{6437664000}`秒；这个也是为什么我们在Ubuntu下获取到的文件系统时间 “不正确“ 的原因。

  ```c++
  # fs_ops.cc gcc filesystem实现
  namespace
  {
    struct internal_file_clock : fs::__file_clock
    {
      using __file_clock::_S_to_sys;
      using __file_clock::_S_from_sys;
  
  #ifdef _GLIBCXX_HAVE_SYS_STAT_H
      static fs::file_time_type
      from_stat(const fs::stat_type& st, std::error_code& ec) noexcept
      {
        const auto sys_time = fs::file_time(st, ec);
        if (sys_time == sys_time.min())
          return fs::file_time_type::min();
        return _S_from_sys(sys_time);
      }
  #endif
    };
  }
  
  fs::file_time_type
  fs::last_write_time(const path& p, error_code& ec) noexcept
  {
  #ifdef _GLIBCXX_HAVE_SYS_STAT_H
    return do_stat(p, ec,
  		 [&ec](const auto& st) {
  		     return internal_file_clock::from_stat(st, ec);
  		 },
  		 file_time_type::min());
  #else
    ec = std::make_error_code(std::errc::function_not_supported);
    return file_time_type::min();
  #endif
  }
  
  
  # c++17 gcc源码 chrono.h
  #if __cplusplus >= 201703L
    namespace filesystem
    {
      struct __file_clock
      {
        using duration                  = chrono::nanoseconds;
        using rep                       = duration::rep;
        using period                    = duration::period;
        using time_point                = chrono::time_point<__file_clock>;
        static constexpr bool is_steady = false;
  
        static time_point
        now() noexcept
        { return _S_from_sys(chrono::system_clock::now()); }
  
  #if __cplusplus > 201703L
        template<typename _Dur>
  	static
  	chrono::file_time<_Dur>
  	from_sys(const chrono::sys_time<_Dur>& __t) noexcept
  	{ return _S_from_sys(__t); }
  
        // For internal use only
        template<typename _Dur>
  	static
  	chrono::sys_time<_Dur>
  	to_sys(const chrono::file_time<_Dur>& __t) noexcept
  	{ return _S_to_sys(__t); }
  #endif // C++20
  
      private:
        using __sys_clock = chrono::system_clock;
  
        // This clock's (unspecified) epoch is 2174-01-01 00:00:00 UTC.
        // A signed 64-bit duration with nanosecond resolution gives roughly
        // +/- 292 years, which covers the 1901-2446 date range for ext4.
        static constexpr chrono::seconds _S_epoch_diff{6437664000};
  
      protected:
        // For internal use only
        template<typename _Dur>
  	static
  	chrono::time_point<__file_clock, _Dur>
  	_S_from_sys(const chrono::time_point<__sys_clock, _Dur>& __t) noexcept
  	{
  	  using __file_time = chrono::time_point<__file_clock, _Dur>;
  	  return __file_time{__t.time_since_epoch()} - _S_epoch_diff;
  	}
  
        // For internal use only
        template<typename _Dur>
  	static
  	chrono::time_point<__sys_clock, _Dur>
  	_S_to_sys(const chrono::time_point<__file_clock, _Dur>& __t) noexcept
  	{
  	  using __sys_time = chrono::time_point<__sys_clock, _Dur>;
  	  return __sys_time{__t.time_since_epoch()} + _S_epoch_diff;
  	}
      };
    } // namespace filesystem
  #endif // C++17
  ```

- 修正方法

  - 方法1：使用 `sys/stat.h` 替换 filesystem::last_write_time 接口的使用

    ```c++
    #include <sys/stat.h> // stat
    
    int64_t get_path_last_write_time(const fs::path &p)
    {
        struct stat file_stat;
    
        auto filepath = p.string();
        if (stat(filepath.c_str(), &file_stat) == 0) {
            return static_cast<int64_t>(file_stat.st_mtime);
        }
    
        return 0;
    }
    ```

  - 方法2：记录file_time_type和system time的时间偏差，使用时，进行修正（c++20的转换接口）

    ```c++
    # file clock: 2174-01-01 00:00:00 UTC ext4文件系统
    auto sys_time = std::chrono::system_clock::now().time_since_epoch();
    auto file_time = std::filesystem::file_time_type::clock::now().time_since_epoch();
    auto elapse = std::chrono::duration_cast<std::chrono::seconds>(sys_time - file_time).count();
    
    # unbuntu下输出实际为6437663999；精度误差导致
    elapse: 6437664000
    ```

- 后记：c++20提供了file_time_type到system_clock的转换

  ```c++
  auto file_time = std::filesystem::file_time_type::clock::now();
  auto sys_time = std::chrono::file_clock::to_sys(file_time); // file_time_type转换为system_time
  std::cout << "file clock rep as sys = " << std::chrono::duration_cast<std::chrono::seconds>(sys_time.time_since_epoch()).count() << std::endl;
  ```

- 教训：对于跨平台的文件系统，应该了解它们的计时规则（e.g. windows从1960开始），不能单纯的直接转换file_time_type、system_clock。

