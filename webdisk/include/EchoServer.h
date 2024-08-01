#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "ThreadPool.h"
#include "TcpServer.h"
//#include "Configuration.h"
#include "Dictionary.h"
#include "json.hpp"
#include "WebPageQuery.h"
#include <map>
#include <vector>
#include <fstream>
#include "CacheManager.h"
#include <sstream>

using ConfigurationPtr = shared_ptr<Configuration>;
using DictionaryPtr = shared_ptr<Dictionary>;
using WebPageQueryPtr = shared_ptr<WebPageQuery>;
using namespace std;

class MyTask
{
public:
    MyTask(int flag,const string &msg, const TcpConnectionPtr &con,const ConfigurationPtr &conf,const DictionaryPtr &dic,const WebPageQueryPtr &web);
    void process(int idx);
private:
    int _flag;
    string _msg;
    TcpConnectionPtr _con;
    ConfigurationPtr _configure;
    DictionaryPtr _dic;
    WebPageQueryPtr _web;
    vector<string> _candicate;//候选词
};

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize
               , const string &ip
               , unsigned short port);
    ~EchoServer();

    //服务器的启动与停止
    void start();
    void stop();
    void InputDict(const ConfigurationPtr &conf);

    //三个回调
    void onNewConnection(const TcpConnectionPtr &con);
    void onMessage(const TcpConnectionPtr &con);
    void onClose(const TcpConnectionPtr &con);

private:
    ThreadPool _pool;
    TcpServer _server;
    ConfigurationPtr _configure;
    DictionaryPtr _dic;
    WebPageQueryPtr _web;
    // vector<string> _candicate;//候选词
};

#endif
