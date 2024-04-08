#ifndef RTSPSERVER_SOCKETSOPS_H
#define RTSPSERVER_SOCKETSOPS_H

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <string>

namespace sockets {

// socket fd
int  createTcpSock();
int  bind(int sockfd, std::string ip, uint16_t port);
int  listen(int sockfd, int backlog);
int  accept(int listenfd, bool setnonblock, bool setignoresigpipe);
void close(int sockfd);

int send(int sockfd, const void* buf, int size, int flags = 0);  // TCP
// int sendto(int sockfd, const void* buf, int len, const struct sockaddr *destAddr); // udp

// fd flags
int setBlock(int sockfd, int writeTimeout = 0);  // 设置阻塞模式
int setNonBlock(int sockfd);
int setReuseAddr(int sockfd, int on);
int setReusePort(int sockfd);
int setCloseOnExec(int sockfd);

// ip-port
std::string getPeerIp(int sockfd);
int16_t     getPeerPort(int sockfd);
int         getPeerAddr(int sockfd, struct sockaddr_in* addr);

// signal
int setIgnoreSigPipe(int socketfd);
}  // namespace sockets

#endif