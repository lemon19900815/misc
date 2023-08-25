## 1 pip windows & linux

```
豆瓣源：http://pypi.douban.com/simple
清华源：https://pypi.tuna.tsinghua.edu.cn/simple
阿里云：http://mirrors.aliyun.com/pypi/simple

# 设置pip使用的源
pip config set global.index-url http://pypi.douban.com/simple
pip config set global.trusted-host pypi.douban.com/simple

# pip临时使用某个源
pip install package -i source_url --trusted-host=source_url
```

## 2 centos安装yum源

### 2.1 centos安装yum源

```sh
# 备份原repo
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.bak
# 下载centos7的repo文件
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
# 把里面所有的http地址都改成https的
vim /etc/yum.repos.d/CentOS-Base.repo

# 清除缓存
yum clean all
# 生成缓存
yum makecache 
# 更新yum
yum update
```

### 2.2 centos安装epel源（相当于yum额外的软件包）
在安装epel源之前，先安装epel-release软件包，它会自动配置yum仓库

```sh
# 安装软件包
yum install -y epel-release
# 增加epel源
wget -O /etc/yum.repos.d/epel-7.repo http://mirrors.aliyun.com/repo/epel-7.repo

# 清除缓存
yum clean all
# 生成缓存
yum makecache 
# 更新yum
yum update
```

## 3 ubuntu更换国内源

### 3.1 更换国内清华源（阿里源有问题）

```sh
# 备份
cp /etc/apt/source.list /etc/apt/source.list.bak

# 编辑，删除所有内容
vim /etc/apt/source.list

# 填入清华源
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse

# 更新缓存
apt-get update
apt-get upgrade
```



