## Linux命令(centos)

- 查看系统信息

  ```sh
  # 列出所有版本信息
  lsb_release -a
  
  # 适用于所有的Linux发行版
  cat /etc/issue
  
  # 提供系统的基本概览，包括操作系统名称、内核版本、主机名等
  hostnamectl
  ```

- 查看端口占用
  - 方式1：`lsof -i:8080`
  - 方式2：`netstat -tunlp|grep 8080` 
    - -t(--tcp): 
    - -u(--udp): 
    - -n(--numeric): 显示数字形式地址而不是去解析主机、端口或用户名
    - -l(--listening): 只显示正在侦听的套接字(这是默认的选项)
    - -p(--program):  显示套接字所属进程的PID和名称

- 测试http接口响应

  - Linux下使用curl（post必须携带参数: -d ''）

    `curl -X POST -d '' "url"`

  - Windows下使用postman工具

  - 通过管道传递数据（注意参数：**-d @-**）

    `cat 1.txt | curl -X POST -d @- "url"`
  
- 查看系统内核：`uname –a`

- Temporary failure in name resolution 错误解决方法:

  ```
  vim /etc/resolv.conf
  
  '添加了两行（DNS地址是咨询了服务器运营商得知的）'
  nameserver   202.96.209.133
  nameserver   61.129.88.123
  nameserver 114.114.114.114
  ```

- gdb设置环境变量
  - 命令参数 ` (gdb) set args XXXX`
  - 环境变量：`(gdb) set environment LD_LIBRARY_PATH=.`
  - 本地运行：`export LD_LIBRARY_PATH=.;$LD_LIBRARY_PATH`
  
- xargs使用（-i {}）

  ```sh
  # 基础使用
  find . -name "*.h" | xargs grep "aaa" -
  
  # 使用-i {}转移
  cmd1 | xargs -i cmd2 {} xxx
  
  # 在文件开头新增内容
  cat license.txt a.h > temp && mv temp a.h
  echo "what's the fuck" | cat - a.h > temp && mv temp a.h
  
  # 查找所有指定后缀的文件添加license.txt
  find . -name "*.h" -print0 | xargs -0 -I{} bash -c 'cat license.txt {} > {}.tmp && mv {}.tmp {}'
  
  # 查找所有文件添加license.txt
  # 这里-I{}定义了一个替换字符串，sh -c用于执行带有变量的命令。
  # **注意事项**
  # - 当使用xargs处理大量数据时，要注意不要超过命令行长度限制。
  # - 使用-I选项时，确保替换字符串不会与命令中的其他部分冲突。
  # - 在处理包含特殊字符的文件名时，总是使用-0选项和NUL字符分隔符。
  find . -type f \( -name "*.cppp" -o -name "*.h" \) -print0 | xargs -0 -I{} bash -c 'cat license.txt {} > {}.tmp && mv {}.tmp {}'
  ```
  
  
  
  - ``
  - 在文件开头新增内容：
  - 查找所有指定后缀的文件添加文件头：
  
- 下载yum安装包（yum下载的包保存到本地）

  ```ini
  #下载yumdownloadonly插件(yum可能已经携带)
  yum install yum-plugin-downloadonly
  
  #yum 下载rpm包到指定目录，只下载不安装
  yum install --downloadonly --downloaddir=路径  安装包名
  实例：yum install  --downloadonly --downloaddir=/usr/local perf
  #安装1
  rpm -Uvh --force --nodeps *.rpm
  实例：rpm -Uvh --force --nodeps perf-3.10.0-1160.76.1.el7.x86_64.rpm
  #安装2：可以直接使用报名进行安装
  yum intall perf-3.10.0-1160.76.1.el7.x86_64.rpm
  ```

- top命令 

  ```ini
  # 进入之后按f键显示每一列的含义；
  # shift+e切换Mem、Swap显示单位
  
  load average: 0.00, 0.01, 0.05 
  # 系统负载，即任务队列的平均长度。 三个数值分别为 1分钟、5分钟、15分钟前到现在的平均值。
  
  Cpu参数详解：
  	us: 用户占用cpu百分比（平均，可以使用1键切换显示具体每个cpu的占用百分比情况）
  	sy: 系统占用cpu百分比
  	ni: 用户进程空间内改变过优先级的进程占用CPU百分比
  	id: cpu空闲占比
  	wa: 等待输入输出的CPU时间百分比
  	hi: 硬中断（Hardware IRQ）占用CPU的百分比
  	si: 软中断（Software Interrupts）占用CPU的百分比
  	st: 用于有虚拟cpu的情况，用来指示被虚拟机偷掉的cpu时间
  ```

  [top命令解释参考]: https://blog.csdn.net/xujiamin0022016/article/details/89072116

  

## 动态库

- 动态库的版本号命名规则

  共享库的命名必须，如 `libname.so.x.y.z`， 最前面使用前缀 `lib`，中间是库的名字和后缀 `.so`，最后三个数字是版本号。

  - x-主版本号(不兼容)：重大升级，不同主版本的库之间的库是不兼容的。
  - y-次版本号(向下兼容): 增量升级，增加一些新的接口但保留原有接口。高次版本号的库向后兼容低次版本号的库。
  - z-发布版本号(相互兼容)：库的一些诸如错误修改、性能改进等，不添加新接口，也不更改接口。主版本号和次版本号相同的前提下，不同发布版本之间完全兼容。

- cmake设置动态库版本信息

  - `VERSION` 指代动态库版本

  - `SOVERSION `指代 API 版本

  - 示例

    ```
    # MakeLists.txt中设置 VERSION & SOVERSION
    set_target_properties(hello PROPERTIES VERSION 1.1.0 SOVERSION 1)
    
    # 动态库目录会生成以下三个文件
    libhello.so.1.1.0
    libhello.so.1->libhello.so.1.1.0
    libhello.so ->libhello.so.1
    ```

- zip压缩命令（-r 递归进入目录 ; -y 保留软连接，默认是压缩实际指向的文件）

  `zip -ry lib.zip lib`

## 其他

- vscode选项建议

  - linux远程（不能直接修改intelliSenseCacheSize选项；通过修改文件的方式来实现）

    ```
    # 直接修改不生效
    "C_Cpp.intelliSenseCacheSize": 0
    
    # 需要修改文件.vscode-server/extensions/ms-vscode.cpptools-1.10.8/package.json中选项C_Cpp.intelliSenseCacheSize的默认值为0
    
    # 修改之后需要重启vscode服务
    ```

  - windows可以直接修改选项`C_Cpp.intelliSenseCacheSize`来达到目的

