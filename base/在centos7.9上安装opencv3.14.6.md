# Linux ：Centos7 使用新版的gcc - Developer Toolset 7

`yum install centos-release-scl`
`yum install devtoolset-7`
`scl enable devtoolset-7 bash`

#source /opt/rh/devtoolset-7/enable

# 在centos7.9上安装opencv3.14.6

- 安装gtk2.0
  `yum install libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev`

  `yum  install libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev`

- openssl依赖
  `yum install openssl`
  `ln -s /usr/lib64/libssl.so.1.0.2k /usr/lib64/libssl.so`
  `ln -s /usr/lib64/libcrypto.so.1.0.2k /usr/lib64/libcrypto.so`

- openblas依赖（源epel.repo）
  `yum install -y epel-release`
  `yum install openblas-devel.x86_64 `
  `ln -s /usr/lib64/libopenblas-r0.3.3.so /usr/lib64/libopenblas.so`

- ubuntu安装opencv无法下载IPPICV的问题 ippicv_2020_lnx_intel64_20191018_general.tgz

  ```
  # 使用迅雷下载ippicv
  https://github.com/opencv/opencv_3rdparty/blob/ippicv/master_20191018/ippicv/ippicv_2020_lnx_intel64_20191018_general.tgz
  
  # 修改opencv/3rdparty/ippicv/ippicv.cmake中的下载地址为本地
  #https://raw.githubusercontent.com/opencv/opencv_3rdparty/${IPPICV_COMMIT}/ippicv/
  file:/root/opensource/opencv-3.4.16/3rdparty/ippicv/
  ```

- 安装 gstreamer（也可以关闭-DWITH_GSTREAMER=OFF）

  `yum install gstreamer-devel.x86_64 gstreamer1-devel.x86_64  gstreamer-plugins-base-devel.x86_64 `

- 关闭部分不需要的选项
  `cmake ..  -DBUILD_DOCS=OFF  -DBUILD_EXAMPLES=OFF  -DINSTALL_BIN_EXAMPLES=OFF  -DBUILD_TESTS=OFF  -DBUILD_opencv_apps=OFF -DWITH_1394=OFF`

# 安装opencv：3.4.4

可以参考：[centos安装教程](https://learnopencv.com/install-opencv-3-4-4-on-centos-7/)

```ini
sudo yum -y install epel-release
sudo yum -y install git gcc gcc-c++ cmake3
sudo yum -y install qt5-qtbase-devel
sudo yum install -y python34 python34-devel python34-pip
sudo yum install -y python python-devel python-pip

sudo yum -y install python-devel numpy python34-numpy
sudo yum -y install gtk2-devel

sudo rpm --import http://li.nux.ro/download/nux/RPM-GPG-KEY-nux.ro
sudo rpm -Uvh http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm
sudo yum install -y ffmpeg
sudo yum install -y ffmpeg-devel

sudo yum install -y libpng-devel
sudo yum install -y jasper-devel
sudo yum install -y openexr-devel
sudo yum install -y libwebp-devel
sudo yum -y install libjpeg-turbo-devel
sudo yum install -y freeglut-devel mesa-libGL mesa-libGL-devel
sudo yum -y install libtiff-devel
sudo yum -y install libdc1394-devel
sudo yum -y install tbb-devel eigen3-devel
sudo yum -y install boost boost-thread boost-devel
sudo yum -y install libv4l-devel
sudo yum -y install gstreamer-plugins-base-devel
```

