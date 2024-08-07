# ARM交叉编译环境构建

参考链接：

1. [Linux安装配置ARM交叉编译器arm-linux-gnueabihf-gcc](https://blog.csdn.net/qq_40296728/article/details/135458955)



## 1. 前置说明

以ubuntu14.04 32位系统为例，安装gcc7.5版本用于支持c++17；

1. 从[ubuntu镜像官网](https://releases.ubuntu.com/trusty/)下载对应的版本，这里我们选择ubuntu14.04的32位系统：ubuntu-14.04.6-desktop-i386.iso；

2. 下载交叉编译工具链[arm-linux-gnueabihf-7.5](https://releases.linaro.org/components/toolchain/binaries/7.5-2019.12/arm-linux-gnueabihf/)

   ```markdown
   # gcc相关，注意选择32位的版本
   gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf.tar.xz
   runtime-gcc-linaro-7.5.0-2019.12-arm-linux-gnueabihf.tar.xz
   sysroot-glibc-linaro-2.25-2019.12-arm-linux-gnueabihf.tar.xz
   
   export PATH=$PATH:/path/to/your/directory/arm-toolchains/gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/bin
   ```

   

3. 安装ubuntu14.04版本的虚拟机；

4. 在虚拟机中安装ssh服务器，外部方便使用`MobaXterm/xshell`等工具；

   ```sh
   # 在虚拟机内部安装openssh-server
   sudo apt-get install openssh-server
   
   # 默认情况下，SSH服务安装后通常会自动启动。
   ```

   

5. 读取可执行文件的依赖动态库信息；

   ```sh
   # 交叉编译环境不能使用ldd查看依赖信息，所以通过arm-linux-gnueabihf-readelf命令读取依赖的库和rpath信息
   # 读取所有信息
   arm-linux-gnueabihf-readelf -d a.out
   
   # 读取rpath
   arm-linux-gnueabihf-readelf -d a.out | grep -i path
   
   # 读取依赖库
   arm-linux-gnueabihf-readelf -d a.out | grep -i needed
   ```

   

6. 编译程序验证：交叉编译之后的程序我们需要运行验证是否能够正确运行，如果没有好的方式，我们只能打包编译好的程序到目标机器上进行，这样通常会延长开发调试周期，极大的增加时间成本。

   ```sh
   # 通过安装qemu用户空间模拟方式，可以让程序得到运行
   sudo apt-get install qemu-user
   
   # 使用交叉编译工具链编译测试程序，然后使用qemu-arm进行验证
   
   # 目标机器运行时，通常会出现以下错误：
   # /lib/ld-linux-armhf.so.3: No such file or directory
   qemu-arm a.out
   
   # 需要指定交叉编译工具链的/lib/ld-linux-armhf.so.3目录，通过find找到动态库目录，再指定路径
   # 可能输出：../gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/ld-linux-armhf.so.3
   find / -name ld-linux-armhf.so.3
   
   # 这时应该可以得到正确的运行结果
   qemu-arm -L ../gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc a.out
   
   # 导出变量或者在~/.bashrc尾部增加以下行
   export qemu_3rd=/path/to/your/directory/arm-toolchain/gcc-linaro-7.5.0-2019.12-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc
   
   # 设置alias运行交叉编译的程序
   echo "alias qemu-run='qemu-arm -L $qemu_3rd'" >> ~/.bashrc && source ~/.bashrc
   # 运行arm程序
   qemu-run ./a.out
   ```

   

7. 模拟软件调试（分为2个步骤）

   - 构建gdb-server：`qemu-arm -L $qemu_3rd -g 1234 a.out`；
   - 使用gdb连接server：`arm-linux-gnueabihf-gdb ./a.out`，进入gdb之后输入`target remote localhost:1234`；

   ```sh
   root@ubuntu:~/arm-toolchains/test/build$ arm-linux-gnueabihf-gdb ./a.out 
   GNU gdb (Linaro_GDB-2017.10) 8.0.1.20171114-git
   Copyright (C) 2017 Free Software Foundation, Inc.
   License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
   This is free software: you are free to change and redistribute it.
   There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
   and "show warranty" for details.
   This GDB was configured as "--host=i686-pc-linux-gnu --target=arm-linux-gnueabihf".
   Type "show configuration" for configuration details.
   For bug reporting instructions, please see:
   <http://www.gnu.org/software/gdb/bugs/>.
   Find the GDB manual and other documentation resources online at:
   <http://www.gnu.org/software/gdb/documentation/>.
   For help, type "help".
   Type "apropos word" to search for commands related to "word"...
   Reading symbols from ./a.out...done.
   (gdb) target remote localhost:1234 <----------------------输入
   Remote debugging using localhost:1234
   warning: remote target does not support file transfer, attempting to access files from local filesystem.
   warning: Unable to find dynamic linker breakpoint function.
   GDB will be unable to debug shared library initializers
   and track explicitly loaded dynamic code.
   0x40801a40 in ?? ()
   (gdb) c <----------------------输入
   Continuing.
   warning: Could not load shared library symbols for 9 libraries, e.g. /lib/libpthread.so.0.
   Use the "info sharedlibrary" command to see the complete listing.
   Do you need "set solib-search-path" or "set sysroot"?
   
   Breakpoint 1, main (argc=1, argv=0x407ffed4) at /path/to/your/directory/arm-toolchains/test/src/main.cpp:11
   11	    logger my_logger("test", true);
   (gdb) c
   Continuing.
   ```



其他相关知识：

- ARM vs ARMEB
  - ARM： 这代表ARM Little-Endian模式，是ARM架构处理器最常用和默认的字节序；
  - ARMEB：这代表ARM Big-Endian模式，与Little-Endian相反，最高有效字节（Most Significant Byte, MSB）存储在最低地址，随后的字节按地址递增逆序存储。

- 更换源：[Ubuntu 14.04更换国内软件源的方法](https://cloud.baidu.com/article/3177203)

  ```ini
  deb http://mirrors.aliyun.com/ubuntu/ trusty main restricted
  deb http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted
  deb http://mirrors.aliyun.com/ubuntu/ trusty universe
  deb http://mirrors.aliyun.com/ubuntu/ trusty-updates universe
  deb http://mirrors.aliyun.com/ubuntu/ trusty multiverse
  deb http://mirrors.aliyun.com/ubuntu/ trusty-updates multiverse
  deb http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
  deb http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted
  deb http://mirrors.aliyun.com/ubuntu/ trusty-security universe
  deb http://mirrors.aliyun.com/ubuntu/ trusty-security multiverse
  ```



## 2. 编译工具

1. 安装[gcc-7.5](https://mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-7.5.0/gcc-7.5.0.tar.xz)； [参考链接](https://blog.csdn.net/weixin_42608165/article/details/93740862)

   ```sh
   # 当前系统的gcc的版本是4.8，版本较低需要升级为7.5版本
   
   # 1. 编译gcc之前，需要安装旧的g++
   sudo apt-get install g++
   
   # 2. 安装前置依赖
   sudo apt-get install libmpc-dev libgmp-dev libmpfr-dev
   
   # 3. 下载gcc版本
   wget https://mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-7.5.0/gcc-7.5.0.tar.xz
   
   # 4. 解压
   tar -xvf gcc-7.5.0.tar.xz
   
   # 5. 配置编译
   cd gcc-7.5.0
   ./configure --prefix=/opt/gcc-7.5.0 --enable-checking=release --enable-bootstrap --enable-languages=c,c++ --enable-bootstrap
   make -j4
   
   sudo make install
   ```

   

2. 安装[cmake3.29](https://objects.githubusercontent.com/github-production-release-asset-2e65be/537699/adff2bbc-e2da-4b7f-8179-a30759f92d20?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=releaseassetproduction%2F20240529%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240529T095832Z&X-Amz-Expires=300&X-Amz-Signature=014380e871b8cc3ab61a26dbb0ac6b65923d536bcdc09a6149735dbc9ea264c7&X-Amz-SignedHeaders=host&actor_id=7922557&key_id=0&repo_id=537699&response-content-disposition=attachment%3B%20filename%3Dcmake-3.29.3.tar.gz&response-content-type=application%2Foctet-stream)

   ```sh
   # 1. 下载cmake源码包
   
   # 安装依赖
   sudo apt install libssl-dev
   
   # 如果bootstrap在运行的过程中出现GLIBCXX符号不对，需要将刚才编译的gcc符号链接重新设置
   sudo ln -sf /usr/local/lib/libstdc++.so.6.0.24 /usr/lib/i386-linux-gnu/libstdc++.so.6
   
   # 编译安装
   ./bootstrap
   
   make j4
   sudo make install
   
   # **使用cmake进行交叉编译，指定cxx编译器**
   cmake .. -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++
   ```

   

3. 设置cmake交叉编译工具链：toolchains-arm.cmake

   ```cmake
   SET(CMAKE_SYSTEM_NAME Linux)
   SET(CMAKE_SYSTEM_PROCESSOR arm)
   
   # 指定到libc目录
   set(CMAKE_SYSROOT /path/to/your/directory/arm-toolchains/gcc-linaro-5.5.0-2017.10-i686_arm-linux-gnueabihf/arm-linux-gnueabihf/libc)
   SET(tools /path/to/your/directory/arm-toolchains/gcc-linaro-5.5.0-2017.10-i686_arm-linux-gnueabihf/bin)
   SET(CMAKE_C_COMPILER ${tools}/arm-linux-gnueabihf-gcc)
   SET(CMAKE_CXX_COMPILER ${tools}/arm-linux-gnueabihf-g++)
   SET(CMAKE_CROSSCOMPILING ON)
   SET(CMAKE_INSTALL_PREFIX "/path/to/your/directory/arm-toolchains/third_party" CACHE PATH "Installation Prefix Path") # $third_party
   
   # 在编译时指定
   export cmake_arm_toolchain=/path/to/your/directory/arm-toolchains/third_party/toolchains-arm.cmake
   cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/toolchains-arm.cmake
   ```

   

4. 



## 3. 交叉编译第三方库

1. 编译openssl1.1.1t

   ```shell
   # 1.使用wget或者在github下载版本
   wget https://codeload.github.com/openssl/openssl/tar.gz/refs/tags/OpenSSL_1_1_1t
   
   # 2.解压软件包
   tar -zxvf openssl1.1.1t.tar.gz & cd openssl1.1.1t
   
   # 3.使用config生成makefile文件
   ./config no-asm shared no-async --prefix=$(pwd)/install --cross-compile-prefix=arm-linux-gnueabihf-
   
   # 4.移除makefile文件中的-m32
   vim Makefile
   
   # 5.执行编译
   make & make install
   
   no-asm:  在交叉编译过程中不使用汇编代码代码加速编译过程；
   shared: 生成动态连接库。
   no-async: 交叉编译工具链没有提供GNU C的ucontext库
   --prefix=: 安装路径
   --cross-compile-prefix=: 交叉编译工具
   ```

   

2. 编译paho.mqtt.c-1.3.12

   ```sh
   # 从github下载mqtt版本文件
   wget https://codeload.github.com/eclipse/paho.mqtt.c/tar.gz/refs/tags/v1.3.12
   
   # 解压
   tar -zxvf paho.mqtt.c-1.3.12.tar.gz & cd paho.mqtt.c-1.3.12.tar.gz
   
   # 编译
   mkdir build & cd build
   
   # 设置变量；也可以在~/.bashrc的末尾添加，添加之后运行source ~/.bashrc使变量生效
   third_party=/path/to/your/directory/arm-toolchains/third_party
   
   # CMAKE_C_COMPILER：使用gcc编译，需要指定交叉编译的gcc工具链
   # CMAKE_CXX_COMPILER：使用g++编译，需要指定交叉编译的g++工具链
   # OPENSSL_ROOT_DIR：指定查找openssl的路径
   # PAHO_WITH_SSL：启动ssl
   cmake .. -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR=$third_party -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_INSTALL_PREFIX=$third_party
   
   make -j4
   ```

   

3. 编译mysql5.7.34

   [交叉编译MySQL的【C API (libmysqlclient)】](https://blog.csdn.net/lc315yuhuofei/article/details/103711181?ops_request_misc=&request_id=&biz_id=102&utm_term=mysql%205.7%20%E6%BA%90%E7%A0%81%E7%BC%96%E8%AF%91libmysqlclient&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-9-103711181.142^v100^pc_search_result_base4&spm=1018.2226.3001.4187)

   ```sh
   # 下载mysql connector/c版本
   wget https://cdn.mysql.com/archives/mysql-connector-c/mysql-connector-c-6.1.11-src.tar.gz
   
   # 解压
   tar -zxvf mysql-connector-c-6.1.11-src.tar.gz & cd mysql-connector-c-6.1.11-src
   
   # 1.编译主机平台（可能是x86或者x64的ubuntu平台）
   mkdir build && cd build && cmake ..
   
   # 这里应该会遇到以下错误：原文链接：https://blog.csdn.net/szy759590387/article/details/129793889
   CMake Error at cmake/install_macros.cmake:365 (ENDIF):
     Flow control statements are not properly nested.
   Call Stack (most recent call first):
     CMakeLists.txt:199 (INCLUDE)
   
   # 交换cmake/install_macros.cmake的第365，366行
   # 再次编译安装
   cmake .. && make -j4
   sudo make install
   
   # 2.编译arm交叉平台库（**使用2.2编译**）
   cd ../ && mkdir build_arm && cd build_arm
   # 2.1 -DCMAKE_CROSSCOMPILING=ON这里的选项不会生效，不知道什么地方被修改了，请用指定交叉工具链文件的方式编译
   cmake .. -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ -DCMAKE_INSTALL_PREFIX=$third_party -DCMAKE_CROSSCOMPILING=ON
   
   # 2.2 指定交叉编译cmake工具链
   cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchains-arm.cmake -DCMAKE_INSTALL_PREFIX=$third_party -DSTACK_DIRECTION=1 -DHAVE_LLVM_LIBCPP=OFF -DHAVE_C_FLOATING_POINT_OPTIMIZATION_PROBLEMS=OFF -DHAVE_CXX_FLOATING_POINT_OPTIMIZATION_PROBLEMS=OFF
   
   # 编译会遇到如下错误：
   ./comp_err: 1: ./comp_err: Syntax error: word unexpected (expecting ")")
   
   # 拷贝主机平台刚才编译的build/extra文件comp_err
   cp -f ../build/extra/comp_err extra
   
   # 这里重新编译仍然会出现上述错误，主要原因在于在编译时再次生成了comp_err，需要禁用掉
   # 从extra/CMakeLists.txt中可以看到可以通过CMAKE_CROSSCOMPILING变量定义过滤掉，
   # 但是没有生效，指定交叉编译工具链之后，在交叉编译下才不会去编译comp_err，否则重新生成它会导致上述错误
   # 也可以简单的把cmake修改为 IF(0)屏蔽掉comp_err的编译
   ​```txt
   	IF(NOT CMAKE_CROSSCOMPILING)
        ADD_EXECUTABLE(comp_err comp_err.c)
        TARGET_LINK_LIBRARIES(comp_err mysys mysys_ssl)
        SET_TARGET_PROPERTIES(comp_err PROPERTIES LINKER_LANGUAGE CXX)
       ENDIF()
   ​```
   
   # 设置path路径
   export PATH=$PATH:`pwd`/extra && make -j4
   ```

   

4. 编译sqlite3

   ```sh
   # 项目中直接使用sqlite源码，直接编译到项目代码中，编译时会出现如下错误：
   
   # sqlite3.c.o: undefined reference to symbol 'dlsym@@GLIBC_2.4'
   
   # 在链接库中加入dl即可解决该问题
   
   target_link_libraries(${PROJECT_NAME} dl)
   ```
   
   
   
5. 编译boost-1.70

   [csdn-boost交叉编译参考](https://blog.csdn.net/qq_37434641/article/details/129606410)

   [boost-4.3. Configuration](https://www.boost.org/doc/libs/1_82_0/tools/build/doc/html/index.html)

   [交叉编译boost context出现的问题](https://blog.csdn.net/A707471534/article/details/122213791)

   ```sh
   # 下载对应版本：
   wget https://master.dl.sourceforge.net/project/boost/boost/1.70.0/boost_1_70_0.7z?viasf=1
   
   # 安装7z解压工具
   sudo apt install 7z
   
   # 解压文件
   7zr x boost_1_70_0.7z
   
   # 运行boostrap.sh生成b2
   ./boostrap.sh --prefix=./install
   
   # 错误信息：主要会遇到以下2个错误
   # 1. 禁用关于版本的相关行：屏蔽977~980这几行
   boost_1_70_0/tools/build/src/tools\common.jam:977: in toolset-tag
   *** argument error
   * rule numbers.less ( n1 n2 )
   * called with: ( 4 )
   * missing argument n2
   
   # 2. 修改abi：<abi>aapcs => <abi>sysv
   alias asm_sources
   : asm/make_arm_aapcs_elf_gas.S
   asm/jump_arm_aapcs_elf_gas.S
   asm/ontop_arm_aapcs_elf_gas.S
   : <abi>sysv
       <address-model>32
       <architecture>arm
       <binary-format>elf
       <toolset>gcc
       ;
   
   alias asm_sources
   : asm/make_arm64_aapcs_elf_gas.S
   asm/jump_arm64_aapcs_elf_gas.S
   asm/ontop_arm64_aapcs_elf_gas.S
   : <abi>sysv
       <address-model>64
       <architecture>arm
       <binary-format>elf
       <toolset>gcc
       ;
   
   # 修改生成的project-config.jam文件
   if ! gcc in [ feature.values <toolset> ]
   {
       using gcc ;
       using gcc : arm : arm-linux-gnueabihf-g++ : <cxxflags>-std=c++11 ; # 新增，指定c++11编译，注意尾部的空格
   }
   
   # 交叉编译boost，link=[static|shared]
   ./b2 install --prefix=./install --build-dir=./build link=shared variant=debug variant=release address-model=32 toolset=gcc-arm
   
   # 编译本地环境
   ./b2 install --prefix=./install/local --build-dir=./build/local link=static runtime-link=static address-model=32 toolset=gcc
   
   # 编译交叉编译版本：**请安装到--prefix=$third_party**
   ./b2 install --prefix=./install/arm --build-dir=./build/arm link=static runtime-link=static address-model=32 toolset=gcc-arm
   
   # install vs stage
   # install会安装include，而stage不会，stage只会将编译库放在指定的路径下
   
   # 可以看到在 Linux 下并没有区分 debug/release 版本, 是否支持多线程等命名标识。于是在 StackOverflow 上查到了相关提问：在 Unix 或 Linux 下，自 Boost1.40.0 中就已经默认取消了库的名称修饰，如果需要名称修饰的话，则添加 b2 参数 --layout=tagged
   ```

   

6. 编译zlib

   ```sh
   # wget https://codeload.github.com/madler/zlib/tar.gz/refs/tags/v1.2.13
   
   tar -zxvf zlib-1.2.13.tar.gz
   cd zlib-1.2.13 && mkdir build && cd build
   
   # 交叉编译工具链中指定了CMAKE_INSTALL_PREFIX，所以这里不用指定安装目录
   cmake .. -DCMAKE_TOOLCHAIN_FILE=$cmake_arm_toolchain && make -j4 && make install
   ```

   

7. 编译libevent-2.1.12

   ```sh
   # 下载libevent版本：下载到third_party目录中
   wget https://objects.githubusercontent.com/github-production-release-asset-2e65be/1856976/1524bb00-bedd-11ea-8c51-a1125df41e13?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=releaseassetproduction%2F20240612%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240612T072909Z&X-Amz-Expires=300&X-Amz-Signature=776778f242a47af75409da4edc243458b0ed3b9fd0c8cc0fdd96accc4526b587&X-Amz-SignedHeaders=host&actor_id=7922557&key_id=0&repo_id=1856976&response-content-disposition=attachment%3B%20filename%3Dlibevent-2.1.12-stable.tar.gz&response-content-type=application%2Foctet-stream
   
   tar -zxvf libevent-2.1.12-stable.tar.gz
   cd libevent-2.1.12-stable && mkdir build && cd build
   
   # 交叉编译
   cmake .. -DCMAKE_TOOLCHAIN_FILE=$cmake_arm_toolchain -DOPENSSL_ROOT_DIR=$third_party -DZLIB_ROOT=$third_party
   ```

   

8. 编译thrift-0.12

   ```sh
   # 安装依赖
   sudo apt install flex bison
   
   # 安装flex之后可能会导致cmake错误，这是由于新cmake依赖与gcc-7.5，重新建立软连接即可
   cmake: /usr/lib/i386-linux-gnu/libstdc++.so.6: version `CXXABI_1.3.9' not found (required by cmake)
   cmake: /usr/lib/i386-linux-gnu/libstdc++.so.6: version `CXXABI_1.3.11' not found (required by cmake)
   cmake: /usr/lib/i386-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.22' not found (required by cmake)
   cmake: /usr/lib/i386-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.20' not found (required by cmake)
   cmake: /usr/lib/i386-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by cmake)
   
   # 重建软连接，解决上面的错误
   sudo ln -sf /usr/local/lib/libstdc++.so.6.0.24 /usr/lib/i386-linux-gnu/libstdc++.so.6
   
   # 只编译c++的thrift库，其他的全部禁止掉
   cmake .. -DCMAKE_TOOLCHAIN_FILE=$cmake_arm_toolchain -DOPENSSL_ROOT_DIR=$third_party -DZLIB_ROOT=$third_party -DBOOST_ROOT=$third_party -DLIBEVENT_ROOT=$third_party -DBUILD_PYTHON=OFF -DBUILD_JAVA=OFF -DBUILD_C_GLIB=OFF -DBUILD_HASKELL=OFF -DBUILD_TESTING=OFF -DBUILD_COMPILER=OFF
   ```

   

9. 

## 4. 编译arm版本

- 使用build.sh脚本编译工程arm版本：`sh build.sh [debug|release]`；

  ```sh
  #!/bin/bash
  
  # current script path.
  script_path=$(cd "$(dirname $0)";pwd)
  
  cd $script_path && cd ..
  
  build_type="Debug"
  
  case $1 in
    [Dd]ebug)
      ;;
    [Rr]elease)
      build_type="Release"
      ;;
    *)
      echo "Usage: $0 {debug|release}"
      exit
      ;;
  esac
  
  echo "build project with $build_type mode."
  
  if [ ! -e "target/arm32-linux" ]; then
      cd target && tar -zxvf arm32-linux.tar.gz && cd ..
  fi
  
  export third_party=$(pwd)/target/arm32-linux
  echo "third_party: $third_party"
  
  # remove build dir.
  [ -d "build" ] && rm -rf build
  
  mkdir build && cd build
  
  # build the target.
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm_toolchain.cmake -DCMAKE_BUILD_TYPE=$build_type
  make -j4
  
  unset third_party
  
  # cmake pack.
  #cpack -C CPackSourceConfig.cmake
  
  echo "===========build $build_type ok==========="
  ```

  

- 使用deploy.sh脚本打包软件：`sh deploy.sh`，*打包时会修改程序的rpath目录*；

  ```sh
  #!/bin/bash
  
  # current script path.
  script_path=$(cd "$(dirname $0)";pwd)
  
  cd $script_path && cd ..
  
  # 项目名
  proj=xxx
  
  if [ ! -e "config/$proj" ]; then
      echo "not exist config dir: config/$proj"
      exit 1
  fi
  
  # remove exist proj dir.
  [ -d "$proj" ] && rm -rf $proj
  
  # 程序/文件夹名
  target=buerjia-$proj
  
  # 创建打包目录
  mkdir -p $target && cd $target
  
  # 拷贝目标软件
  cp ../build/a.out $target
  
  # 读取程序的rpath目录
  rpath=`readelf -d $target | grep -i path | cut -d'[' -f2 | cut -d']' -f1`
  echo "rpath: $rpath"
  
  # 拷贝配置信息
  cp ../config/$proj . -rf
  mv $proj config
  
  # 读取依赖库
  libs=`readelf -d $target | grep -i needed | cut -d'[' -f2 | cut -d']' -f1 | xargs echo`
  
  # 忽略系统库：不在rpath目录下的库认为是系统库
  echo "dependency libs list:"
  for lib in $libs; do
      if [ -e "$rpath/$lib" ]; then
      #    echo "    ignore: $lib"
      #else
          echo "    $lib"
      fi
  done
  
  # 修改目标rpath
  new_rpath="./"
  
  # 拷贝所有依赖库，并修改rpath
  echo "copy all dependency."
  for lib in $libs; do
      if [ -e "$rpath/$lib" ]; then
          cp $rpath/$lib . && chrpath -r $new_rpath $lib 1>/dev/null
      fi
  done
  
  # 最后修改目标软件的rpath
  chrpath -r $new_rpath $target
  
  # 打包
  now=`date +%Y%m%d-%H%M%S`
  tar_file=$target-$now.tar.gz
  
  # package
  cd .. && tar -zcvf $tar_file $target
  
  echo "deploy $tar_file ok."
  ```

  

- 模拟软件运行请参考模拟运行；

  


## 5. 编译x64版本

```sh
sudo ln -sf /usr/local/lib/libstdc++.so.6.0.24 /usr/lib/i386-linux-gnu/libstdc++.so.6
sudo ln -sf /usr/local/bin/gcc /usr/bin/cc
sudo ln -sf /usr/local/bin/gcc /usr/bin/gcc
sudo ln -sf /usr/local/bin/g++ /usr/bin/g++

# 编译本地库，参考arm环境对依赖的编译

# 编译openssl
./config shared --prefix=$third_party_local && make -j4 && make install

# 编译mqtt
cmake .. -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR=$third_party_local -DCMAKE_INSTALL_PREFIX=$third_party_local && make -j4 && make install

# 编译thrift
cmake .. -DOPENSSL_ROOT_DIR=$third_party_local -DBOOST_ROOT=/path/to/your/directory/arm-toolchains/third_party/boost_1_70_0/install/local -DBUILD_PYTHON=OFF -DBUILD_JAVA=OFF -DBUILD_C_GLIB=OFF -DBUILD_HASKELL=OFF -DBUILD_TESTING=OFF -DBUILD_COMPILER=OFF -DCMAKE_INSTALL_PREFIX=$third_party_local
```



## 6. 其他注意事项

1. 注意使用2.0版本的qemu-user模拟arm程序运行时，会出现：`Unsupported setsockopt level=1 optname=13`从而导致模拟软件运行失败（13 表示level=1表示SOL_SOCKET ，optname=13表示SO_LINGER选项）；

   ```sh
   Thrift: Fri Jun 14 16:34:23 2024 TServerSocket::listen() setsockopt() SO_LINGER : Protocol not available
   exception: Could not set SO_LINGER: Protocol not available
   ```

   

3. qemu-arm运行时报出段错误

   ```
   /build/qemu-sKi_Nc/qemu-2.0.0+dfsg/tcg/tcg.c:1693: tcg fatal error
   段错误 (核心已转储)
   ```

4. 

   

​    

​    

​    