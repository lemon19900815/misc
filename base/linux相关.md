## Linux命令(centos)
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

  - `cmd1 | xargs -i cmd2 {} xxx`

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

