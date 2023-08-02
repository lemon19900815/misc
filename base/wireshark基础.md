# wireshark使用基础

## 软件基础使用

1、选择需要抓包的网卡（**重要**），选错网卡不会有数据。

2、填写过滤指令

### 过滤指令

```sh
# 协议过滤
http
tcp
udp
icmp

# 端口过滤
tcp.port==80
tcp.srcport==80
tcp.dstport==80
udp.port==80
udp.srcport==80
udp.dstport==80

# ip过滤
ip.src_host==127.0.0.1
ip.dst_host==127.0.0.1

# 多个指令使用and、or
ip.src_host==127.0.0.1 and tcp.dstport==80
```

