# perf使用

- `perf record/report`

  ```ini
  # -e cpu-clock指定分析事件；可以使用perf list查看支持的事件;
  # -p pid指定分析进程；选填
  perf record -e cpu-clock -p pid
  
  # 根据record生成的perf.data，生成报告;默认为perf.data
  perf report [-i input]
  ```

- `perf top`

  ```ini
  # 对于一个指定的性能事件（默认是CPU周期）显示消耗最多的函数/命令
  # 直接使用该命令直接分析
  # -p pid指定需要分析的进程；选填
  perf top [-p pid]
  ```

- `perf stat`

  ```ini
  # 分析程序的性能概况
  perf stat [-p pid]
  ```

  引用：[perf的基本使用方法](https://blog.csdn.net/jasonactions/article/details/109332167?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166434355916782428620384%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=166434355916782428620384&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~top_positive~default-1-109332167-null-null.nonecase&utm_term=perf&spm=1018.2226.3001.4450)

