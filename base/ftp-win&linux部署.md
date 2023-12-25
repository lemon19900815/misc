[TOC]

# ftp部署

## 1. windows部署ftp

**reference**：[windows部署ftp](https://www.jb51.net/article/257408.htm)

## 2. linux部署ftp

**reference: **

1. [ubuntu下vsftpd部署](https://blog.csdn.net/qq_24568487/article/details/89400561?ops_request_misc=&request_id=&biz_id=102&utm_term=ubuntu%20vsftpd%E6%90%AD%E5%BB%BA&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-2-89400561.nonecase&spm=1018.2226.3001.4187)

2. [Linux-FTP文件服务器](https://blog.csdn.net/qq_45277554/article/details/131452806)

### 2.1 安装vsftpd

```sh
# 安装vsftpd
sudo apt install vsftpd

# 查看帮助:vsftpd
man vsftpd

# 查看帮助:vsftpd.conf
man vsftpd.conf
```

### 2.2 修改配置

```ini
# 注意：=前后不能有空格
# 可通过egrep -v "^#" /etc/vsftpd.conf查看开启的选项
# 设置之后用户登录的ftp目录显示为根目录/;否则ftp登录用户可能会看到所有目录信息，不满足最小化权限原则
chroot_local_user=YES
#chroot_list_enable=YES
#chroot_list_file=/etc/vsftpd.chroot_list

# 本机认证用户是否能够登录，新建的用户(e.g. lemon)是属于local user
local_enable=YES

# ftp用户能够执行写入
write_enable=YES

# ftp的工作目录定义；默认为/srv/ftp
local_root=/data
```

### 2.3 新建用户

```sh
# 3.1 新建登录账号(e.g. lemon)
# -m会自动创建用户的home目录
sudo useradd -m -d /home/lemon -s /usr/sbin/nologin lemon

# 3.2 给lemon添加密码
sudo passwd lemon

# 3.3 vsftpd的新增用户设置bash为/usr/sbin/nologin没有办法登录ftp问题
# ftp会根据/etc/shells这个文件来判断一个用户是否是有效用户，会阻止那些shell不在/etc/shells里的用户登陆.
# 修改/etc/pam.d/vsftpd，注释掉PAM模块对vsftp的过度验证
vim /etc/pam.d/vsftpd
#auth   required        pam_shells.so
```

### 2.4 服务重启

```sh
sudo service vsftpd restart|stop|start
```

### 2.5 ftp登录

```sh
root@ubuntu20:/srv/ftp# ftp 127.0.0.1
Connected to 127.0.0.1.
220 (vsFTPd 3.0.5)
Name (127.0.0.1:vboxuser): myftp
331 Please specify the password.
Password:
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> ls
200 PORT command successful. Consider using PASV.
150 Here comes the directory listing.
-rw-r--r--    1 1001     1001            4 Dec 22 16:26 1.txt
drwxr-xr-x    2 1001     1001         4096 Dec 25 14:39 mydata
226 Directory send OK.
ftp> pwd
257 "/" is the current directory
ftp> bye
```

### 2.6 完整配置

```ini
# Example config file /etc/vsftpd.conf
#
# The default compiled in settings are fairly paranoid. This sample file
# loosens things up a bit, to make the ftp daemon more usable.
# Please see vsftpd.conf.5 for all compiled in defaults.
#
# READ THIS: This example file is NOT an exhaustive list of vsftpd options.
# Please read the vsftpd.conf.5 manual page to get a full idea of vsftpd's
# capabilities.
#
#
# Run standalone?  vsftpd can run either from an inetd or as a standalone
# daemon started from an initscript.
listen=NO
#
# This directive enables listening on IPv6 sockets. By default, listening
# on the IPv6 "any" address (::) will accept connections from both IPv6
# and IPv4 clients. It is not necessary to listen on *both* IPv4 and IPv6
# sockets. If you want that (perhaps because you want to listen on specific
# addresses) then you must run two copies of vsftpd with two configuration
# files.
listen_ipv6=YES
#
# Allow anonymous FTP? (Disabled by default).
anonymous_enable=NO
#
# Uncomment this to allow local users to log in.
local_enable=YES
#
# Uncomment this to enable any form of FTP write command.
write_enable=YES
#
# Default umask for local users is 077. You may wish to change this to 022,
# if your users expect that (022 is used by most other ftpd's)
#local_umask=022
#
# Uncomment this to allow the anonymous FTP user to upload files. This only
# has an effect if the above global write enable is activated. Also, you will
# obviously need to create a directory writable by the FTP user.
#anon_upload_enable=YES
#
# Uncomment this if you want the anonymous FTP user to be able to create
# new directories.
#anon_mkdir_write_enable=YES
#
# Activate directory messages - messages given to remote users when they
# go into a certain directory.
dirmessage_enable=YES
#
# If enabled, vsftpd will display directory listings with the time
# in  your  local  time  zone.  The default is to display GMT. The
# times returned by the MDTM FTP command are also affected by this
# option.
use_localtime=YES
#
# Activate logging of uploads/downloads.
xferlog_enable=YES
#
# Make sure PORT transfer connections originate from port 20 (ftp-data).
connect_from_port_20=YES
#
# If you want, you can arrange for uploaded anonymous files to be owned by
# a different user. Note! Using "root" for uploaded files is not
# recommended!
#chown_uploads=YES
#chown_username=whoever
#
# You may override where the log file goes if you like. The default is shown
# below.
#xferlog_file=/var/log/vsftpd.log
#
# If you want, you can have your log file in standard ftpd xferlog format.
# Note that the default log file location is /var/log/xferlog in this case.
#xferlog_std_format=YES
#
# You may change the default value for timing out an idle session.
#idle_session_timeout=600
#
# You may change the default value for timing out a data connection.
#data_connection_timeout=120
#
# It is recommended that you define on your system a unique user which the
# ftp server can use as a totally isolated and unprivileged user.
#nopriv_user=ftpsecure
#
# Enable this and the server will recognise asynchronous ABOR requests. Not
# recommended for security (the code is non-trivial). Not enabling it,
# however, may confuse older FTP clients.
#async_abor_enable=YES
#
# By default the server will pretend to allow ASCII mode but in fact ignore
# the request. Turn on the below options to have the server actually do ASCII
# mangling on files when in ASCII mode.
# Beware that on some FTP servers, ASCII support allows a denial of service
# attack (DoS) via the command "SIZE /big/file" in ASCII mode. vsftpd
# predicted this attack and has always been safe, reporting the size of the
# raw file.
# ASCII mangling is a horrible feature of the protocol.
#ascii_upload_enable=YES
#ascii_download_enable=YES
#
# You may fully customise the login banner string:
#ftpd_banner=Welcome to blah FTP service.
#
# You may specify a file of disallowed anonymous e-mail addresses. Apparently
# useful for combatting certain DoS attacks.
#deny_email_enable=YES
# (default follows)
#banned_email_file=/etc/vsftpd.banned_emails
#
# You may restrict local users to their home directories.  See the FAQ for
# the possible risks in this before using chroot_local_user or
# chroot_list_enable below.
# 用户登录pwd显示根目录为/而非用户的/home/$u目录
chroot_local_user=YES
#
# You may specify an explicit list of local users to chroot() to their home
# directory. If chroot_local_user is YES, then this list becomes a list of
# users to NOT chroot().
# (Warning! chroot'ing can be very dangerous. If using chroot, make sure that
# the user does not have write access to the top level directory within the
# chroot)
#chroot_local_user=YES
#chroot_list_enable=YES
# (default follows)
#chroot_list_file=/etc/vsftpd.chroot_list
#
# You may activate the "-R" option to the builtin ls. This is disabled by
# default to avoid remote users being able to cause excessive I/O on large
# sites. However, some broken FTP clients such as "ncftp" and "mirror" assume
# the presence of the "-R" option, so there is a strong case for enabling it.
#ls_recurse_enable=YES
#
# Customization
#
# Some of vsftpd's settings don't fit the filesystem layout by
# default.
#
# This option should be the name of a directory which is empty.  Also, the
# directory should not be writable by the ftp user. This directory is used
# as a secure chroot() jail at times vsftpd does not require filesystem
# access.
secure_chroot_dir=/var/run/vsftpd/empty
#
# This string is the name of the PAM service vsftpd will use.
#pam_service_name=vsftpd
check_shell=NO
#
# This option specifies the location of the RSA certificate to use for SSL
# encrypted connections.
rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
ssl_enable=NO

#
# Uncomment this to indicate that vsftpd use a utf8 filesystem.
#utf8_filesystem=YES
#
# ftp的根目录，默认根目录为/srv/ftp，也可自行设置
local_root=/srv/ftp
```

