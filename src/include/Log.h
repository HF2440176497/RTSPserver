
#ifndef RTSPSERVER_LOG_H
#define RTSPSERVER_LOG_H
#include <time.h>
#include <string>
#include <vector>

// 日志模块

/**
 * @brief 格式化并返回时间信息字符串
*/
static std::string getTime() {
    const char* time_fmt = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);
    char time_str[64];
    strftime(time_str, sizeof(time_str), time_fmt, localtime(&t));
    return time_str;
}


/**
 * @brief 从文件路径中获取最后的文件名
 * @param file_path 例如 "/data/video.h264"
 * @todo 可加入异常处理
*/
static std::string getFile(std::string file_path) {

    std::string pattern = "/";
    int path_len = file_path.size();
    std::string::size_type pos;

    for (int i = path_len - 1; i >= 0; --i) {
        pos = file_path.find(pattern, i);  // 查找位置 i 后面匹配字符串的位置
        if (pos == std::string::npos)  
            continue;
        break;
    }
    std::string ret = file_path.substr(pos + 1, path_len - pos);
    return ret;
}   


#define LOGI(format, ...)  fprintf(stderr,"[INFO]%s [%s:%d] " format "\n", getTime().data(),__FILE__,__LINE__,##__VA_ARGS__)
#define LOGE(format, ...)  fprintf(stderr,"[ERROR]%s [%s:%d] " format "\n",getTime().data(),__FILE__,__LINE__,##__VA_ARGS__)

// format 指定格式的字符串

#endif