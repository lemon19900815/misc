/* networking.c -- Basic TCP socket stuff made a bit less boring
*
* Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include <sys/types.h>

#ifndef _WIN32
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define sErrorNo errno
#define sEAGAIN EAGAIN
#define sEINTR EINTR
#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN32
#define ANET_NOTUSED(V) V
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mstcpip.h>
#define close closesocket
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#define sErrorNo WSAGetLastError()
#define sEAGAIN WSAEWOULDBLOCK
#define sEINTR WSAEINTR
#endif

#include "networking.h"

namespace network
{
  static void anetSetError(char *err, const char *fmt, ...)
  {
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, ANET_ERR_LEN, fmt, ap);
    va_end(ap);
  }

  int anetSetBlock(char *err, int fd, int non_block)
  {
#ifdef _WIN32
    u_long mode = non_block;
    if (0 != ioctlsocket(fd, FIONBIO, &mode))
      return ANET_ERR;
#else
    int flags;
    /* Set the socket blocking (if non_block is zero) or non-blocking.
    * Note that fcntl(2) for F_GETFL and F_SETFL can't be
    * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
      anetSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
      return ANET_ERR;
    }

    if (non_block)
      flags |= O_NONBLOCK;
    else
      flags &= ~O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
      anetSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
      return ANET_ERR;
    }
#endif
    return ANET_OK;
  }

  int anetNonBlock(char *err, int fd)
  {
    return anetSetBlock(err, fd, 1);
  }

  int anetBlock(char *err, int fd)
  {
    return anetSetBlock(err, fd, 0);
  }

  /* Set TCP keep alive option to detect dead peers. The interval option
  * is only used for Linux as we are using Linux-specific APIs to set
  * the probe send time, interval, and count. */
  int anetKeepAlive(char *err, int fd, int interval)
  {
    int val = 1;

#ifdef _WIN32
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&val, sizeof(val)) == -1) {
      anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
      return ANET_ERR;
    }

    tcp_keepalive alive;
    DWORD dwBytesRet = 0;
    alive.onoff = TRUE;
    alive.keepalivetime = interval * 1000;
    /* According to http://msdn.microsoft.com/en-us/library/windows/desktop/ee470551(v=vs.85).aspx
    On Windows Vista and later, the number of keep-alive probes (data retransmissions) is set to 10 and cannot be changed.
    So we set the keep alive interval as interval/10, as 10 probes will be send before
    detecting an error
    */
    val = interval / 10;
    if (val == 0) val = 1;
    alive.keepaliveinterval = val * 1000;
    if (WSAIoctl(fd, SIO_KEEPALIVE_VALS, &alive, sizeof(alive),
      NULL, 0, &dwBytesRet, NULL, NULL) == SOCKET_ERROR) {
      anetSetError(err, "WSAIotcl(SIO_KEEPALIVE_VALS) failed with error code %d\n", WSAGetLastError());
      return ANET_ERR;
    }
#else
    /* Default settings are more or less garbage, with the keepalive time
    * set to 7200 by default on Linux. Modify settings to make the feature
    * actually useful. */

    /* Send first probe after interval. */
    val = interval;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
      anetSetError(err, "setsockopt TCP_KEEPIDLE: %s\n", strerror(errno));
      return ANET_ERR;
    }

    /* Send next probes after the specified interval. Note that we set the
    * delay as interval / 3, as we send three probes before detecting
    * an error (see the next setsockopt call). */
    val = interval / 3;
    if (val == 0) val = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
      anetSetError(err, "setsockopt TCP_KEEPINTVL: %s\n", strerror(errno));
      return ANET_ERR;
    }

    /* Consider the socket in error state after three we send three ACK
    * probes without getting a reply. */
    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
      anetSetError(err, "setsockopt TCP_KEEPCNT: %s\n", strerror(errno));
      return ANET_ERR;
    }
#endif

    return ANET_OK;
  }

  static int anetSetTcpNoDelay(char *err, int fd, int val)
  {
#ifdef _WIN32
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) == -1) {
#else
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == -1) {
#endif
      anetSetError(err, "setsockopt TCP_NODELAY: %s", strerror(errno));
      return ANET_ERR;
    }
    return ANET_OK;
  }

  int anetEnableTcpNoDelay(char *err, int fd)
  {
    return anetSetTcpNoDelay(err, fd, 1);
  }

  int anetDisableTcpNoDelay(char *err, int fd)
  {
    return anetSetTcpNoDelay(err, fd, 0);
  }

  int anetSetSendBuffer(char *err, int fd, int buffsize)
  {
#ifdef _WIN32
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&buffsize, sizeof(buffsize)) == -1) {
#else
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1) {
#endif
      anetSetError(err, "setsockopt SO_SNDBUF: %s", strerror(errno));
      return ANET_ERR;
    }
    return ANET_OK;
  }

  int anetTcpKeepAlive(char *err, int fd)
  {
    int yes = 1;

#ifdef _WIN32
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&yes, sizeof(yes)) == -1) {
      anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
      return ANET_ERR;
    }
#else
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)) == -1) {
      anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
      return ANET_ERR;
    }
#endif

    return ANET_OK;
  }

  /* Set the socket send timeout (SO_SNDTIMEO socket option) to the specified
  * number of milliseconds, or disable it if the 'ms' argument is zero. */
  int anetSendTimeout(char *err, int fd, unsigned long long ms)
  {
    timeval tv;

    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;

#ifdef _WIN32
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) == -1) {
      anetSetError(err, "setsockopt SO_SNDTIMEO: %s", strerror(errno));
      return ANET_ERR;
    }
#else
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
      anetSetError(err, "setsockopt SO_SNDTIMEO: %s", strerror(errno));
      return ANET_ERR;
    }
#endif

    return ANET_OK;
  }

  /* anetGenericResolve() is called by anetResolve() and anetResolveIP() to
  * do the actual work. It resolves the hostname "host" and set the string
  * representation of the IP address into the buffer pointed by "ipbuf".
  *
  * If flags is set to ANET_IP_ONLY the function only resolves hostnames
  * that are actually already IPv4 or IPv6 addresses. This turns the function
  * into a validating / normalizing function. */
  int anetGenericResolve(char *err, char *host, char *ipbuf, size_t ipbuf_len,
    int flags)
  {
    addrinfo hints, *info;
    int rv;

    memset(&hints, 0, sizeof(hints));
    if (flags & ANET_IP_ONLY) hints.ai_flags = AI_NUMERICHOST;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;  /* specify socktype to avoid dups */

    if ((rv = getaddrinfo(host, NULL, &hints, &info)) != 0) {
      anetSetError(err, "%s", gai_strerror(rv));
      return ANET_ERR;
    }
    if (info->ai_family == AF_INET) {
      sockaddr_in *sa = (sockaddr_in *)info->ai_addr;
      inet_ntop(AF_INET, &(sa->sin_addr), ipbuf, ipbuf_len);
    }
    else {
      sockaddr_in6 *sa = (sockaddr_in6 *)info->ai_addr;
      inet_ntop(AF_INET6, &(sa->sin6_addr), ipbuf, ipbuf_len);
    }

    freeaddrinfo(info);
    return ANET_OK;
  }

  int anetResolve(char *err, char *host, char *ipbuf, size_t ipbuf_len)
  {
    return anetGenericResolve(err, host, ipbuf, ipbuf_len, ANET_NONE);
  }

  int anetResolveIP(char *err, char *host, char *ipbuf, size_t ipbuf_len)
  {
    return anetGenericResolve(err, host, ipbuf, ipbuf_len, ANET_IP_ONLY);
  }

  static int anetSetReuseAddr(char *err, int fd)
  {
    int yes = 1;
    /* Make sure connection-intensive things like the redis benckmark
    * will be able to close/open sockets a zillion of times */
#ifdef _WIN32
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes)) == -1) {
#else
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
#endif
      anetSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
      return ANET_ERR;
    }
    return ANET_OK;
    }

  static int anetCreateSocket(char *err, int domain)
  {
    int s;
    if ((s = socket(domain, SOCK_STREAM, 0)) == -1) {
      anetSetError(err, "creating socket: %s", strerror(errno));
      return ANET_ERR;
    }

    /* Make sure connection-intensive things like the redis benchmark
    * will be able to close/open sockets a zillion of times */
    if (anetSetReuseAddr(err, s) == ANET_ERR) {
      close(s);
      return ANET_ERR;
    }
    return s;
  }

#define ANET_CONNECT_NONE 0
#define ANET_CONNECT_NONBLOCK 1
  static int anetTcpGenericConnect(char *err, char *addr, int port, int flags)
  {
    int s = ANET_ERR, rv;
    char portstr[6];  /* strlen("65535") + 1; */
    addrinfo hints, *servinfo, *p;

    snprintf(portstr, sizeof(portstr), "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(addr, portstr, &hints, &servinfo)) != 0) {
      anetSetError(err, "%s", gai_strerror(rv));
      return ANET_ERR;
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
      /* Try to create the socket and to connect it.
      * If we fail in the socket() call, or on connect(), we retry with
      * the next entry in servinfo. */
      if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        continue;
      if (anetSetReuseAddr(err, s) == ANET_ERR) goto error;
      if (flags & ANET_CONNECT_NONBLOCK && anetNonBlock(err, s) != ANET_OK)
        goto error;
      if (connect(s, p->ai_addr, p->ai_addrlen) == -1) {
        /* If the socket is non-blocking, it is ok for connect() to
        * return an EINPROGRESS error here. */
        if (errno == EINPROGRESS && flags & ANET_CONNECT_NONBLOCK)
          goto end;
        close(s);
        s = ANET_ERR;
        continue;
      }

      /* If we ended an iteration of the for loop without errors, we
      * have a connected socket. Let's return to the caller. */
      goto end;
    }
    if (p == NULL)
      anetSetError(err, "creating socket: %s", strerror(errno));

  error:
    if (s != ANET_ERR) {
      close(s);
      s = ANET_ERR;
    }
  end:
    freeaddrinfo(servinfo);
    return s;
  }

  // windows socket init
  int anetInitializeNet()
  {
#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data))
      return -1;
#endif

    return 0;
  }

  int anetReleaseNet()
  {
#ifdef _WIN32
    if (WSACleanup())
      return -1;
#endif

    return 0;
  }

  int anetTcpConnect(char *err, char *addr, int port)
  {
    return anetTcpGenericConnect(err, addr, port, ANET_CONNECT_NONE);
  }

  int anetTcpNonBlockConnect(char *err, char *addr, int port)
  {
    return anetTcpGenericConnect(err, addr, port, ANET_CONNECT_NONBLOCK);
  }

  /* Like read(2) but make sure 'count' is read before to return
  * (unless error or EOF condition is encountered) */
  int anetRead(int fd, char *buf, int count)
  {
    int nread, totlen = 0;
    while (totlen != count) {
#ifdef _WIN32
      nread = recv(fd, buf, count - totlen, 0);
#else
      nread = (int)read(fd, buf, count - totlen);                                 /* UPSTREAM_ISSUE: missing (int) cast */
#endif
      if (nread == 0) return totlen;
      if (nread == -1) return -1;
      totlen += nread;
      buf += nread;
    }
    return totlen;
  }

  // read socket buffer once, and return readed char number[<=count]
  int anetRead2(int fd, char* buf, int len)
  {
    int nread;

#ifdef _WIN32
    nread = recv(fd, buf, len, 0);
#else
    nread = (int)read(fd, buf, len);                                 /* UPSTREAM_ISSUE: missing (int) cast */
#endif

    if (nread > 0) return nread; // read success
    if (nread == 0) return -1; // close by peer endpoint

    // nread < 0
    auto ErrNo = sErrorNo;

    // EINTR: interrupt by system, we retry system call. [eg. accept,read,write,select,open can recall, but connect can't recall]
    // EAGAIN: no data has been transferred[timeout or nonblock cause this error], try again later
    // EINTR: if the fd is nonblock, the error wouldn't occur[see: http://willko.iteye.com/blog/1691741]
    if (ErrNo == sEAGAIN || ErrNo == sEINTR) return 0;

    return -1;
  }

  /* Like write(2) but make sure 'count' is read before to return
  * (unless error is encountered) */
  int anetWrite(int fd, char *buf, int count)
  {
    int nwritten, totlen = 0;
    while (totlen != count) {
#ifdef _WIN32
      nwritten = send(fd, buf, count - totlen, 0);
#else
      nwritten = (int)write(fd, buf, count - totlen);                             /* UPSTREAM_ISSUE: missing (int) cast */
#endif
      if (nwritten == 0) return totlen;
      if (nwritten == -1) return -1;
      totlen += nwritten;
      buf += nwritten;
    }
    return totlen;
  }

  int anetWrite2(int fd, char* buf, int len)
  {
    int nwrite;

#ifdef _WIN32
    nwrite = send(fd, buf, len, 0);
#else
    nwrite = write(fd, buf, len);
#endif

    if (nwrite > 0) return nwrite; // write success
    if (nwrite == 0) return -1; // close by peer endpoint

    // nread < 0
    auto ErrNo = sErrorNo;

    // EINTR: interrupt by system, we retry system call. [eg. accept,read,write,select,open can recall, but connect can't recall]
    // EAGAIN: no data has been transferred[timeout or nonblock cause this error], try again later
    // EINTR: if the fd is nonblock, the error wouldn't occur[see: http://willko.iteye.com/blog/1691741]
    if (ErrNo == sEAGAIN || ErrNo == sEINTR) return 0;

    return -1;
  }

  static int anetListen(char *err, int s, sockaddr *sa, socklen_t len, int backlog)
  {
    if (bind(s, sa, len) == -1) {
      anetSetError(err, "bind: %s", strerror(errno));
      close(s);
      return ANET_ERR;
    }

    if (listen(s, backlog) == -1) {
      anetSetError(err, "listen: %s", strerror(errno));
      close(s);
      return ANET_ERR;
    }
    return ANET_OK;
  }

  static int anetV6Only(char *err, int s)
  {
    int yes = 1;
#ifdef _WIN32
    if (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&yes, (int)sizeof(yes)) == -1) {
#else
    if (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &yes, sizeof(yes)) == -1) {
#endif
      anetSetError(err, "setsockopt: %s", strerror(errno));
      close(s);
      return ANET_ERR;
    }
    return ANET_OK;
  }

  static int _anetTcpServer(char *err, int port, char *bindaddr, int af, int backlog)
  {
    int s, rv;
    char _port[6];  /* strlen("65535") + 1 */
    addrinfo hints, *servinfo, *p;

    snprintf(_port, 6, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = af;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    /* No effect if bindaddr != NULL */

    if ((rv = getaddrinfo(bindaddr, _port, &hints, &servinfo)) != 0) {
      anetSetError(err, "%s", gai_strerror(rv));
      return ANET_ERR;
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
      if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        continue;

      if (af == AF_INET6 && anetV6Only(err, s) == ANET_ERR) goto error;
      //if (IF_WIN32(anetSetExclusiveAddr,anetSetReuseAddr)(err,s) == ANET_ERR) goto error;
      if (anetListen(err, s, p->ai_addr, (socklen_t)p->ai_addrlen, backlog) == ANET_ERR) goto error;
      goto end;
    }
    if (p == NULL) {
      anetSetError(err, "unable to bind socket");
      goto error;
    }

  error:
    s = ANET_ERR;
  end:
    freeaddrinfo(servinfo);
    return s;
  }

  int anetTcpServer(char *err, int port, char *bindaddr, int backlog)
  {
    return _anetTcpServer(err, port, bindaddr, AF_INET, backlog);
  }

  int anetTcp6Server(char *err, int port, char *bindaddr, int backlog)
  {
    return _anetTcpServer(err, port, bindaddr, AF_INET6, backlog);
  }

  int anetGenericAccept(char *err, int s, sockaddr *sa, socklen_t *len)
  {
    int fd;
    while (1) {
      if ((fd = accept(s, sa, len)) == -1) {
        if (errno == EINTR)
          continue;
        else {
          anetSetError(err, "accept: %s", strerror(errno));
          return ANET_ERR;
        }
      }
      break;
    }
    return fd;
  }

  int anetTcpAccept(char *err, int s, char *ip, size_t ip_len, int *port)
  {
    int fd;
    sockaddr_storage sa;
    socklen_t salen = sizeof(sa);
    if ((fd = anetGenericAccept(err, s, (sockaddr*)&sa, &salen)) == -1)
      return ANET_ERR;

    if (sa.ss_family == AF_INET) {
      sockaddr_in *s = (sockaddr_in *)&sa;
      if (ip) inet_ntop(AF_INET, (void*)&(s->sin_addr), ip, ip_len);
      if (port) *port = ntohs(s->sin_port);
    }
    else {
      sockaddr_in6 *s = (sockaddr_in6 *)&sa;
      if (ip) inet_ntop(AF_INET6, (void*)&(s->sin6_addr), ip, ip_len);
      if (port) *port = ntohs(s->sin6_port);
    }
    return fd;
  }

  int anetPeerToString(int fd, char *ip, size_t ip_len, int *port)
  {
    sockaddr_storage sa;
    socklen_t salen = sizeof(sa);

    if (getpeername(fd, (sockaddr*)&sa, &salen) == -1) {
      if (port) *port = 0;
      ip[0] = '?';
      ip[1] = '\0';
      return -1;
    }
    if (sa.ss_family == AF_INET) {
      sockaddr_in *s = (sockaddr_in *)&sa;
      if (ip) inet_ntop(AF_INET, (void*)&(s->sin_addr), ip, ip_len);
      if (port) *port = ntohs(s->sin_port);
    }
    else {
      sockaddr_in6 *s = (sockaddr_in6 *)&sa;
      if (ip) inet_ntop(AF_INET6, (void*)&(s->sin6_addr), ip, ip_len);
      if (port) *port = ntohs(s->sin6_port);
    }
    return 0;
  }

  int anetClose(int fd)
  {
#ifdef _WIN32
    int ret = closesocket(fd);
    return ret == 0 ? 0 : -1;
#else
    return close(fd);
#endif
  }

  // listen on port, may both ipv4 and ipv6
  int anetListenToPort(int port, int* fds, int* count)
  {
    *count = 0;

    fds[*count] = anetTcp6Server(nullptr, port, nullptr, TCP_BACKLOG);
    if (fds[*count] != -1) {
      anetNonBlock(nullptr, fds[*count]);
      ++(*count);
    }

    fds[*count] = anetTcpServer(nullptr, port, nullptr, TCP_BACKLOG);
    if (fds[*count] != -1) {
      anetNonBlock(nullptr, fds[*count]);
      ++(*count);
    }

    if (*count) return 0;

    return -1;
  }
}
