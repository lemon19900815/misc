#ifndef __NETWORKING_INC_H__
#define __NETWORKING_INC_H__

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

/* Flags used with certain functions. */
#define ANET_NONE 0
#define ANET_IP_ONLY (1<<0)

// listen tcp backlog
#define TCP_BACKLOG 128

//#define INET_ADDRSTRLEN  22
//#define INET6_ADDRSTRLEN 65
#define IP_ADDRSTRLEN 65

#include <stdio.h>

namespace network
{
  // socket init, support windows
  int anetInitializeNet();
  int anetReleaseNet();
  int anetTcpConnect(char *err, char *addr, int port);
  int anetTcpNonBlockConnect(char *err, char *addr, int port);
  int anetRead(int fd, char *buf, int count);
  // read socket buffer once, and return readed char number[<=count]
  int anetRead2(int fd, char* buf, int count);
  int anetResolve(char *err, char *host, char *ipbuf, size_t ipbuf_len);
  int anetResolveIP(char *err, char *host, char *ipbuf, size_t ipbuf_len);
  int anetTcpServer(char *err, int port, char *bindaddr, int backlog);
  int anetTcp6Server(char *err, int port, char *bindaddr, int backlog);
  int anetTcpAccept(char *err, int serversock, char *ip, size_t ip_len, int *port);
  int anetWrite(int fd, char *buf, int count);
  // write socket buffer once, and return writed char number[<=count]
  int anetWrite2(int fd, char *buf, int count);
  int anetNonBlock(char *err, int fd);
  int anetBlock(char *err, int fd);
  int anetEnableTcpNoDelay(char *err, int fd);
  int anetDisableTcpNoDelay(char *err, int fd);
  int anetTcpKeepAlive(char *err, int fd);
  int anetSendTimeout(char *err, int fd, unsigned long long ms);
  int anetPeerToString(int fd, char *ip, size_t ip_len, int *port);
  int anetKeepAlive(char *err, int fd, int interval);
  int anetClose(int fd);
  // listen on port, may both ipv4 and ipv6
  int anetListenToPort(int port, int* fds, int* count);
}

#endif // __NETWORKING_INC_H__
