
#include "Buffer.h"

#include <sys/uio.h>

#include "SocketsOps.h"

const char* Buffer::kCRLF = "\r\n";

Buffer::Buffer() : mBuffer(kCheapPrepend + initialSize),
                   mReadIndex(kCheapPrepend),
                   mWriteIndex(kCheapPrepend) {
    assert(readableBytes() == 0);
    assert(writableBytes() == initialSize);
    assert(prependableBytes() == kCheapPrepend);
}

Buffer::~Buffer() {
}

/**
 * @brief 写入到 Buffer，返回实际读到的字节数
 */
int Buffer::read(int fd) {

    char extrabuf[65536];
    int  writeable_size = writableBytes();

    iovec riovec[2];
    riovec[0].iov_base = beginWrite();
    riovec[0].iov_len  = writeable_size;

    riovec[1].iov_base = extrabuf;
    riovec[1].iov_len  = sizeof(extrabuf);

    int riocnt = writeable_size < sizeof(extrabuf) ? 2 : 1;  // 数量的确定 根据 writeable 确定是否启用 extrabuf
    int ret    = ::readv(fd, riovec, riocnt);

    if (ret < 0) {
        return -1;
    } else if (ret <= writeable_size) {
        hasWriten(ret);
    } else {  // 此时，extrabuf 存在读取到的数据
        mWriteIndex = mBuffer.size();
        append(extrabuf, ret - writeable_size);
    }
}

int Buffer::write(int fd) {
    return sockets::send(fd, peek(), readableBytes());
}