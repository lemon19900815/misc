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

## console_bar.h

- 通常程序在加载大量数据或者做大量计算时，控制台会陷入到假死的状态，不能观察程序的执行情况
- console bar通过打印任务进度来缓解这种情况
- 运行效果：test\[================================================] 100%...
- 使用示例

```c++
#include <thread>
#include <chrono>

#include "console_bar.h"

// c++11 chrono
static void SleepMs(int64_t ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
    ConsoleBar bar("test", 100);
    for (int32_t i = 0; i < 100; ++i) {
      bar.Process(i+1);
      SleepMs(100);
    }
    return 0;
}
```
  
