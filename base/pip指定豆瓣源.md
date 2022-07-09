# windows & linux下设置豆瓣源：
- pip config set global.index-url http://pypi.douban.com/simple
- pip config set global.trusted-host pypi.douban.com/simple

# pip临时使用豆瓣源：
- pip install package -i http://pypi.douban.com/simple/ --trusted-host=pypi.douban.com/simple

一、安装yum源
1、mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup 备份原repo

2、wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo 下载centos7的repo文件

3、vim /etc/yum.repos.d/CentOS-Base.repo  把里面所有的http地址都改成https的

二、安装epel源（相当于yum额外的软件包）
在安装epel源之前，先安装epel-release软件包，它会自动配置yum仓库

1、yum install -y epel-release

2、wget -O /etc/yum.repos.d/epel-7.repo http://mirrors.aliyun.com/repo/epel-7.repo

三、清除缓存，重新生成缓存
1、yum clean all 清除缓存

2、yum makecache 生成缓存

3、yum update 更新yum
