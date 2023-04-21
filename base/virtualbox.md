# VirtualBox

## 1 创建共享文件夹

- ubuntu20.04->设置->共享文件夹->右侧➕->选择共享文件夹路径&设定共享文件夹名称`share_doc`，勾选自动挂载&固定分配。
- 进入虚拟机系统中，以root权限执行`mount -t vboxsf share_doc /mnt`。

## 2 设置双向复制

- ubuntu20.04->设置->常规->高级->共享粘贴板&拖放修改为双向。

# 3 其他

- host主机键：键盘右侧的ctrl，注意不是左侧的ctrl