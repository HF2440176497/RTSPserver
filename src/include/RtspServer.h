

#ifndef RTSPSERVER_RTSPSERVER_H
#define RTSPSERVER_RTSPSERVER_H

#include <mutex>
#include <vector>
#include <map>
#include <memory>

class Poller;
class RtspConnection;

class RtspServer {

public:
    std::shared_ptr<RtspServer> createNew();

    void start();

private:
    RtspServer(int sockfd);


private:
    int mFd;
    std::vector<int> mDisConnList;
    std::map<int, std::shared_ptr<RtspConnection>> mConnMap;

    std::unique_ptr<Poller> m_Poller;  // Poller 对象理应是单例的，此处的指针应当是独占的
};








#endif