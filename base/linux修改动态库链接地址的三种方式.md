## Linux修改动态库链接地址的三种方式

在Linux下，修改动态库有3种不同的方式：

1. 在`/etc/ld.so.conf.d/`下修改或添加一个我们需要链接的地址，然后使用`ldconfig`使修改生效;

2. 修改LD_LIBRARY_PATH：`export LD_LIBRARY_PATH=new_path:$LD_LIBRARY`;

3. 通过修改生成的可执行程序的RUNPATH来让程序从指定目录搜索动态库。

   ```shell
   # 查看程序或动态库的RUNPATH信息
   readelf -d program
   
   # 设置动态库搜索路径：'$ORIGIN/lib'--这里需要使用单引号
   apt install patchelf
   patchelf --set-rpath '$ORIGIN/lib' program
   
   # $ORIGIN: 程序的当前目录；
   
   # 我们在软件编译之后，进行分发时，通常需要重新指定动态库的链接目录，如果在每一台发布的机器上去修改环境，如果有多台机器需要分发，则需要处理多次，会相当麻烦，通过这种方式修改后的程序，则只需要把动态库一起打包分发即可，在目标机器上无需再次处理动态库环境等问题。
   
   # linux分发软件可以使用linuxdeployqt，它支持qt程序和非qt程序的软件包分发
   ```

   

