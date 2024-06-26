#ifndef RTSPSERVER_BUFFER_H
#define RTSPSERVER_BUFFER_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <vector>

class Buffer {
public:
    Buffer();
    ~Buffer();

public:
    /**
     * @brief 未读取但可读的空间
     */
    int readableBytes() const {
        return mWriteIndex - mReadIndex;
    }

    /**
     * @brief 包括 writeindex 位置在内的的可写空间
     */
    int writableBytes() const {
        return mBuffer.size() - mWriteIndex;
    }

    int prependableBytes() const {
        return mReadIndex;  // readindex = prependable size
    }

    const char* peek() const {
        return begin() + mReadIndex;
    }

    const char* beginWrite() const {
        return begin() + mWriteIndex;
    }

    char* beginWrite() {
        return begin() + mWriteIndex;
    }

    void hasWriten(size_t len) {
        assert(len <= writableBytes());
        mWriteIndex += len;
    }

    /**
     * @brief 撤销指定长度的已写数据
     */
    void unWrite(size_t len) {
        assert(len <= readableBytes());
        mWriteIndex -= len;
    }

    /* 查找字符的相关函数 */

    /**
     * @brief 默认查找范围为 [readindex, writeindex)
     */
    const char* findCRLF() const {
        auto crlf = std::search(peek(), beginWrite(), kCRLF.begin(), kCRLF.end());
        return crlf == beginWrite() ? nullptr : crlf;
    }

    /**
     * @param start 应当位于 [peek, beginWrite) 范围
     * @details 若 start == beginWrite，std::search 同样返回 beginWrite
     */
    const char* findCRLF(const char* start) const {
        assert(start >= peek());
        assert(start <= beginWrite());
        auto crlf = std::search(start, beginWrite(), kCRLF.begin(), kCRLF.end());
        return crlf == beginWrite() ? nullptr : crlf;
    }

    /**
     * @brief 找到最后一个 CRLF
     * @details std::search 和 std::find_end 都是在 [first, last) 区间查找
     */
    const char* findLastCRLF() const {
        auto crlf = std::find_end(peek(), beginWrite(), kCRLF.begin(), kCRLF.end());
        return crlf == beginWrite() ? nullptr : crlf;
    }

    /* 取走数据 */

    void retrieveReadZero() {
        mReadIndex = kCheapPrepend;
    }

    /**
     * @brief 从当前读位置 mReadIndex 检索指定长度的数据
     * @param len 不能超过全部未读取数据的长度
     */
    void retrieve(size_t len) {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            mReadIndex += len;
        } else {            // len == readableBytes，ReadIndex == WriteIndex
            retrieveAll();  // 节省空间 两者归零
        }
    }

    void retrieveAll() {
        mReadIndex  = kCheapPrepend;
        mWriteIndex = kCheapPrepend;
    }

    /**
     * @brief 检索到指定位置
     */
    void retrieveUntil(const char* end) {
        assert(peek() <= end);
        assert(end <= beginWrite());  // end == beginWrite，即 end-peek == readableBytes
        retrieve(end - peek());
    }

    std::string retrieveAsString(size_t len) {
        assert(len <= readableBytes());
        std::string ret = std::string(peek(), len);
        retrieve(len);
        return ret;
    }

    /* 容器内部空间相关 */

    size_t internalCapacity() const {
        return mBuffer.capacity();
    }

    /**
     * @brief 确保有足够的空间 append 调用
     */
    void ensureWritableBytes(size_t len) {
        if (writableBytes() < len) {  // when len == writable, 写入 len 会有：writeindex == Buffer.size()
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    /**
     * @brief 保证 writeable >= len
     */
    void makeSpace(size_t len) {

        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {  // 总空闲空间 < 预留空间
            mBuffer.resize(mWriteIndex + len);
        } else {  // 预留空间足够，进行整理
            assert(kCheapPrepend <= mReadIndex);
            auto readable_size = readableBytes();

            std::copy(begin() + mReadIndex, begin() + mWriteIndex, begin() + kCheapPrepend);
            mReadIndex  = kCheapPrepend;
            mWriteIndex = mReadIndex + readable_size;
            assert(readable_size == readableBytes());
        }
    }
    /**
     * 测试报告：ensureWritableBytes(): assert(writableBytes() >= len); aborted
     * 经过 makeSpace，需要保证 writableBytes() >= len
     * 错误：resize 判断逻辑有误
    */

    void append(std::string str) {
        append(str.data(), str.size());
    }

    /**
     * @brief 将 data 指向区域的数据写入到 Buffer，内部保证
    */
    void append(const char* data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWriten(len);
    }

    void append(const void* data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

public:
    int read(int fd);
    int write(int fd);

private:
    /**
     * @brief 返回堆区缓冲区的起始位置
     */
    const char* begin() const {
        return &*(mBuffer.begin());
    }

    char* begin() {
        return &*(mBuffer.begin());
    }

private:
    std::vector<char>  mBuffer;  // mBuffer 会提供 size(), capacity() 等接口
    int                mReadIndex;
    int                mWriteIndex;

public:
    static const int         kCheapPrepend;
    static const int         initialSize;
    // static const char* kCRLF;
    static const std::string kCRLF;
};

#endif