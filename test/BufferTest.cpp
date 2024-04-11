
#include <gtest/gtest.h>
#include "Buffer.h"

// Buffer 测试程序
// 测试 Buffer 各成员函数可分别进行，因此此处暂无必要使用 Test Fixtures


TEST(BufferTest, AppendRetrieve) {
    
    // constructor
    Buffer buf;
    EXPECT_EQ(buf.readableBytes(), 0);
    EXPECT_EQ(buf.writableBytes(), Buffer::initialSize);
    EXPECT_EQ(buf.prependableBytes(), Buffer::kCheapPrepend);

    // append string
    std::string str1(200, 'x');
    buf.append(str1);
    EXPECT_EQ(buf.readableBytes(), str1.size());
    EXPECT_EQ(buf.writableBytes(), Buffer::initialSize - str1.size());
    EXPECT_EQ(buf.prependableBytes(), Buffer::kCheapPrepend);

    // retrieveAsString()
    const std::string str2 = buf.retrieveAsString(50);
    EXPECT_EQ(str2.size(), 50);
    EXPECT_EQ(buf.readableBytes(), str1.size() - str2.size());  // readindex += len
    EXPECT_EQ(buf.writableBytes(), Buffer::initialSize - str1.size());
    EXPECT_EQ(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());
    EXPECT_EQ(str2, std::string(50, 'x'));  
    // std::string 可直接使用 EXPECT_EQ，Ctring 参考官方文档

}


TEST(BufferTest, FindCRLF) {
    Buffer buf;
    std::string str = std::string("This a server_test \r\n by wanghf \r\n");
    buf.append(str);
    const char* frist_pos = &(*(str.begin() + 19));
    const char* null = nullptr;
    EXPECT_EQ(buf.findCRLF(), frist_pos);
}


// 测试 Buffer 内部空间的增长
TEST(BufferTest, MakeSpace) {
    Buffer buf;
    std::string str1 = std::string(500, 'x');
    std::string str2 = std::string(800, 'y');
    buf.append(str1);
    EXPECT_EQ(buf.readableBytes(), str1.size());
    EXPECT_EQ(buf.writableBytes(), Buffer::initialSize - str1.size());

    buf.append(str2);
    // initialSize = 1024 会引起扩容
    EXPECT_EQ(buf.readableBytes(), str1.size() + str2.size());
    EXPECT_GT(buf.writableBytes(), 0);  // 此时空间增长后，应当 writeindex == size()
}
