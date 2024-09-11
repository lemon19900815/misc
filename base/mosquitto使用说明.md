# mosquitto使用

mqtt服务器broker使用的一些注意事项。

- 服务启动：`mosquitto.exe -c mosquitto.conf`

- 订阅`$SYS/broker`主题的服务状态不能正常收到数据；

  ```sh
  # mosquitto.conf/sys_interval已经配置了刷新频率，但是订阅主题仍然收不到数据
  # 原因：启用了aclfile.txt访问控制权限限制，里面配置了通用主题，但未配置$SYS/broker主题，该主题是特殊的，不在统配符#下
  
  user buerjia
  topic #
  topic $SYS/#     --- 新加行 ---
  ```

  

- 服务的状态可以使用`mqtt.fx`工具订阅，发布消息等和查询broker的状态；



主题通配符说明：

[Mosquitto 手册页 |Eclipse 蚊子](https://mosquitto.org/man/mosquitto-8.html)

- 使用`#`订阅所有主题

  ```sh
  # 订阅所有主题
  mosquitto_sub -t \#
  ```

- 通配符 `+`：a/b/c/d

  - 以下订阅将匹配：
    - `a/b/c/d`
    - `+/b/c/d`
    - `a/+/c/d`
    - `a/+/+/d`
    - `+/+/+/+`
  - 以下订阅将不匹配：
    - `a/b/c`
    - `b/+/c/d`
    - `+/+/+`

- 通配符 `#`：

  - 以下订阅将匹配：
    - `a/b/c/d`
    - `#`
    - `a/#`
    - `a/b/#`
    - `a/b/c/#`
    - `+/b/c/#`