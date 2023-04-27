# vcpkg

## 1 基础使用

### 1.1 安装

- 参考链接：[vcpkg-github](https://github.com/microsoft/vcpkg) & [vcpkg-microsoft](https://learn.microsoft.com/zh-cn/vcpkg/)

- 简单示例

  ```sh
  # 简要使用说明
  git clone https://github.com/microsoft/vcpkg
  
  # windows
  .\vcpkg\bootstrap-vcpkg.bat
  
  # linux
  cd vcpkg && sh ./bootstrap-vcpkg.sh
  
  # 搜索支持的包
  vcpkg search [search term]
  
  # 安装包
  vcpkg install [packages to install]
  
  # linux指定编译动态库[默认安装的是静态库]
  vcpkg install glog[:x64-linux-dynamic]
  
  # windows下默认安装x86版本
  vcpkg install glog[:x64-windows]
  
  # 查看支持的triplet平台
  vcpkg help triplet
  
  # windows集成到vs中
  vcpkg integrate install
  
  # windows删除集成
  vcpkg integrate remove
  ```

### 1.2 triplet
查看支持的triplet（三元组，包括操作系统、CPU架构、c++编译器的组合，用户指定要构建和使用的平台）：`vcpkg help triplet`

- windows
  - 默认编译*动态库*，使用`x[64|86]-windows-static`编译静态库；
  - 默认编译*x86版本*，使用x64-windows编译x64版本；
- linux
  - 默认编译*静态库*，使用x64-linux-dynamic编译动态库；
  - 默认自动根据系统版本选择*[x86|x64]版本编译*；

### 1.3 cmake集成

  ```sh
  # cmake集成vckpg
  cmake .. -DCMAKE_TOOLCHAIN_FILE=D:\src\vcpkg\scripts\buildsystems\vcpkg.cmake
  
  # 在CMakeLists.txt中添加
  include(D:\src\vcpkg\scripts\buildsystems\vcpkg.cmake)
  ```

### 1.4 如何下载指定的库版本？

  - 最新的jsoncpp为1.9.5，我要下载1.9.4的版本；
  - vcpkg.json文件和CMakeLists.txt同级目录；
  - vcpkg支持的版本可以在versions/baseline.json下找到基线版本；
  - 库支持的版本可以从versions/包名的第一个字母/包名.json下找到（如：`versions/j-/jsoncpp.json`）；

  - vcpkg.json配置：注意指定dependencies的version只支持**version>=**这种形式的配置；

    ```json
    {
      "dependencies": [
        "jsoncpp"
      ],
      "overrides": [
        {
          "name": "jsoncpp",
          "version": "1.9.4"
        }
      ],
      "builtin-baseline": "07da05c20c56d3df4b488a16d5bcfeebb761e754"
    }
    ```

  - CMakeLists.txt配置

    ```text
    cmake_minimum_required(VERSION 3.15)
    
    project(test)
    
    # 指定工具链地址
    include(D:\src\vcpkg\scripts\buildsystems\vcpkg.cmake)
    
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    find_package(cxxopts REQUIRED)
    
    add_executable(main main.cpp)
    
    target_link_libraries(main PRIVATE unofficial::sqlite3::sqlite3)
    ```

  - builtin-baseline

    ```sh
    // builtin-baseline生成
    vcpkg x-update-baseline --add-initial-baseline
    
    // builtin-baseline更新
    vcpkg x-update-baseline
    ```

  - 通过vcpkg.json指定依赖库的版本，从而可以每次构建相同的版本

    ```sh
    # windows：在"Developer Command Prompt for VS 2019"下进入到当前工程目录
    cmake ..
    
    # cmake构建时，会先下载依赖，再执行构建
    ```

    