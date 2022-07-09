# c++辅助类Helper

## glog_helper.h

- 依赖glog，需要提前编译好，然后在项目中配置glog路径的信息

- 使用示例

  ```c++
  #include "glog_helper.h"
  
  int main() {
      GlogHelper glog_helper("logname", "logdir");
      
      LOG(INFO) << "test glog system...";
      LOG(WARNING) << "test glog system...";
      LOG(ERROR) << "test glog system...";
      // ...
      
      return 0;
  }
  ```

## dump_helper.h

- 通常部署在windows下的服务在宕机之后很难留下堆栈等信息，所以我们需要一些方式来辅助我们定位，查找问题

- Windows平台宕机生成mini dmp文件，辅助定位问题

- 生成的dump文件会按照当前系统的时间`yyyy-mm-dd-hh-MM-ss.dmp`

- dmp文件可以使用vs工具打开（需要保留编译服务的pdb文件等信息）

- 使用示例

  ```c++
  #include "dump_helper.h"
  
  int main() {
      DumpHelper dump_helper;
      
      return 0;
  }
  ```

  