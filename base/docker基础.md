# Dokcer基础

## 1、容器启动

docker run/start -it 容器名/容器id

## 2、容器退出

- 容器内部退出

  | 退出方式                              | 结果                                           | 再次启动                    |
  | ------------------------------------- | ---------------------------------------------- | --------------------------- |
  | exit（命令）                          | 退出后，这个容器也就消失了，容器销毁ps查询不到 | docker start 容器名/容器id  |
  | Ctrl+D（快捷方式）                    | 退出后，这个容器也就消失了，容器销毁ps查询不到 | docker start 容器名/容器id  |
  | 先按，Ctrl+P;再按，Ctrl+Q（快捷方式） | 退出容器，ps能查询到，还在后台运行             | docker attach 容器名/容器id |

- 容器外部

  docker stop 容器名/容器id

  docker start 容器名/容器id

## 3、docker通过容器构建镜像

- 语法
  `docker commit [OPTIONS] CONTAINER [REPOSITORY[:TAG]]`
  
- OPTIONS 说明：
  - **-a : ** 提交的镜像作者；
  - **-c :** 使用Dockerfile指令来创建镜像；
  - **-m :** 提交时的说明文字；
  - **-p : ** 在commit时，将容器暂停。
  
- 示例（centos7.9容器内构建的编译环境，然后保存下载）

  `docker commit -a "lemon" -m "compiler env" CONTAINER(实例) centos7.9:ce`

## 4、docker镜像存储/加载本地文件

- 存储（tar文件可以再压缩为tar.gz，减少空间）

  `docker save centos7.9:ce > centos7.9-ce.tar`

- 加载

  `docker load < centos7.9-ce.tar`