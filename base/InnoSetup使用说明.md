# InnoSetup使用说明

**参考链接：**

1. 软件下载：https://blog.csdn.net/Viciower/article/details/139035591
2. 使用说明：https://blog.csdn.net/Viciower/article/details/139037455
3. 帮助文档：https://jrsoftware.org/ishelp/
4. 开源地址：https://github.com/jrsoftware/issrc

Inno Setup是Windows程序的免费安装程序。Inno Setup于1997年首次推出，在功能集和稳定性方面与许多商业安装程序相媲美，甚至超过了它们（其他商业的安装包制作工具如InstallSheild）。

对比InstallSheild工具的优点：

1. 开源，免费；
2. 使用、操作简单；



inno setup的脚本示例请参考：[example.iss](./InnoSetup-Example.iss)

- 软件包含多个exe，update目录下的程序为守护进程，通过调用start.vbs启动bin目录下的MyApp.exe;
- 在软件卸载时，通过`InitializeUninstall`定义卸载行为（脚本中卸载前会关闭程序）；
- 关于中文语言包的设置，请参考链接第2条的设置说明；
- 使用`ExpandConstant`展开变量；



start.vbs脚本：解决开机自启动黑框问题；

```vbscript
' 开机自启动软件脚本（bat脚本启动时会存在黑框，所以通过vbs脚本规避）

currentpath = createobject("Scripting.FileSystemObject").GetFile(Wscript.ScriptFullName).ParentFolder.Path
' MsgBox "cur_dir: " & currentpath

' 切换当前工作路径到脚本路径
set ws=WScript.CreateObject("WScript.Shell")
ws.currentdirectory = currentpath

' 启动软件
ws.Run ".\test.exe"
```

