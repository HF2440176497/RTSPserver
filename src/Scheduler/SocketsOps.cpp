
#include "SocketsOps.h"

/**
 * @brief 默认创建阻塞型 socket
 */
int sockets::createTcpSock() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);  // ::socket 引用全局作用域
    return sockfd;
}

/**
 * @brief
 * @param ip "INADDR_ANY" 则接收任意地址绑定到 sockfd；否则绑定到指定 IP 地址
 */
int sockets::bind(int sockfd, std::string ip, uint16_t port) {
    struct sockaddr_in serv_addr {
        0
    };
    serv_addr.sin_family = AF_INET;

    std::string compar_str = "INADDR_ANY";
    if (ip != compar_str) {
        serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    } else {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    serv_addr.sin_port = htons(port);
    if (::bind(sockfd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        return -1;
    }
    return 0;
}

/**
 * @param backlog maximum length to which the queue of pending connections for sockfd
 */
int sockets::listen(int sockfd, int backlog) {
    if (::listen(sockfd, backlog) == -1) {
        return -1;
    }
    return 0;
}

int sockets::accept(int listenfd, bool setnonblock, bool setignoresigpipe) {
    struct sockaddr_in addr;
    socklen_t          addrlen  = sizeof(addr);
    int                acceptfd = ::accept(listenfd, (struct sockaddr*)&addr, &addrlen);
    if (acceptfd == -1) {
        return -1;
    }
    if (setnonblock) {
        setNonBlock(acceptfd);
    }
    if (setignoresigpipe) {
        setIgnoreSigPipe(acceptfd);
    }
    return acceptfd;
}

void sockets::close(int sockfd) {
    int ret = ::close(sockfd);
}

/**
 * @brief 阻塞情况下的发送，若为非阻塞 socket，应当使用 while 循环发送
 */
int sockets::send(int sockfd, const void* buf, int size, int flags) {
    int ret = ::send(sockfd, buf, size, flags);
    return ret;
}

/**
 * @param writeTimeout 发送的超时时间，以 ms 为单位
 */
int sockets::setBlock(int sockfd, int writeTimeout) {
    int old_option = ::fcntl(sockfd, F_GETFL);
    if (fcntl(sockfd, F_SETFL, old_option & (~O_NONBLOCK)) == -1) {  // 去除掉 BLOCK bit

        close(sockfd);
        return -1;
    }
    if (writeTimeout > 0) {
        struct timeval timeout {
            writeTimeout / 1000, (writeTimeout % 1000) * 1000
        };  // 'ms' to 's' and 'us'
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }
    return 0;
}

int sockets::setNonBlock(int sockfd) {
    int old_option = ::fcntl(sockfd, F_GETFL);
    if (fcntl(sockfd, F_SETFL, old_option | O_NONBLOCK) == -1) {  // 去除掉 BLOCK bit

        close(sockfd);
        return -1;
    }
    return 0;
}

int sockets::setReuseAddr(int sockfd, int on) {
    int en_reuseaddr = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&en_reuseaddr, sizeof(int)) == -1) {
        close(sockfd);
        return -1;
    }
    return 0;
}

int sockets::setReusePort(int sockfd) {
    int en_reuseport = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const void*)&en_reuseport, sizeof(int)) == -1) {
        close(sockfd);
        return -1;
    }
    return 0;
}

int sockets::setCloseOnExec(int sockfd) {
    int old_option = ::fcntl(sockfd, F_GETFD);
    if (::fcntl(sockfd, F_SETFD, old_option | O_CLOEXEC)) {
        close(sockfd);
        return -1;
    }
    return 0;
}

int sockets::setIgnoreSigPipe(int sockfd) {
    int en_option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, MSG_NOSIGNAL, &en_option, sizeof(en_option)) == -1) {
        close(sockfd);
        return -1;
    }
    return 0;
}

std::string sockets::getPeerIp(int sockfd) {
    struct sockaddr_in addr    = {0};
    socklen_t          addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr*)&addr, &addrlen) == 0) {
        return inet_ntoa(addr.sin_addr);
    }
    return "0.0.0.0";
}

int16_t sockets::getPeerPort(int sockfd) {
    struct sockaddr_in addr    = {0};
    socklen_t          addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr*)&addr, &addrlen) == 0) {
        return ntohs(addr.sin_port);
    }

    return 0;
}

/**
 * @brief 
 * @param addr 传出参数，caller 负责分配内存
*/
int sockets::getPeerAddr(int sockfd, struct sockaddr_in* addr) {
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getpeername(sockfd, (struct sockaddr*)addr, &addrlen);
}