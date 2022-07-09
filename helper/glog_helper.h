#ifndef __GLOG_HELPRE_H__
#define __GLOG_HELPRE_H__

#include <iostream>
#include <string>

#ifdef WIN32
  #include <direct.h>
#else
  #include <sys/stat.h>
  #include <sys/types.h>
#endif

#include "glog/logging.h"

// glog初始化设置
class GlogHelper finnal {
public:
  explicit GlogHelper(std::string name, std::string log_dir = "../log") {
    // 创建日志输出目录
#ifdef WIN32
    int ret = _mkdir(log_dir.c_str());
#else
    int ret = mkdir(log_dir.c_str(), S_IRGRP | S_IWGRP);
#endif

    if (-1 == ret)
      std::cerr << "mkdir error: " << strerror(errno) << std::endl;

    // Init glog
    FLAGS_colorlogtostderr = true;
    google::InitGoogleLogging(name.c_str());

    google::SetLogDestination(
        google::GLOG_INFO, (log_dir+"/Server_INFO_").c_str());
    google::SetLogDestination(
        google::GLOG_ERROR, (log_dir+"/Server_ERROR_").c_str());
    //google::SetLogDestination(
    //    google::GLOG_WARNING, (log_dir+"/Server_WARNING_").c_str());
    //google::SetLogDestination(
    //    google::GLOG_FATAL, (log_dir+"/Server_FATAL_").c_str());

    // 输出到标准输出的时候大于 INFO 级别的都输出
    google::SetStderrLogging(google::GLOG_INFO);

    std::cout << name << " Log Initialized!" << std::endl;
  }

  ~GLogHelper() {
    std::cout << "Log Shutdown!" << std::endl;

    google::FlushLogFilesUnsafe(0);
    google::ShutdownGoogleLogging();
  }
};

#endif // __GLOG_HELPRE_H__