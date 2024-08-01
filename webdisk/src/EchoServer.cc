#include "../include/EchoServer.h"
#include "../include/TcpConnection.h"
#include "../include/SplitToolCppJieba.h"

#include <iostream>

using std::cout;
using std::endl;

MyTask::MyTask(int flag, const string &msg, const TcpConnectionPtr &con,
               const ConfigurationPtr &conf, const DictionaryPtr &dic, const WebPageQueryPtr &web)
    : _flag(flag), _msg(msg), _con(con), _configure(conf), _dic(dic), _web(web)
{
}

void MyTask::process(int idx)
{
    // 业务逻辑的处理
    SplitToolCppJieba tool;
    if (_flag == 0)
    { // 候选词推荐
        nlohmann::json json_object;
        shared_ptr<CacheManager> cacheManager = CacheManager::getInstance();
        LRUCache &myCache = cacheManager->getCache(idx); // 获取该线程对应的cache

        pair<bool, nlohmann::json> ans = myCache.find(_msg);
        if (!ans.first)
        {
            _candicate = _dic->doQuery(_configure, _msg);
            for (auto &res : _candicate)
            {
                json_object.push_back(res);
            }
        }else{
            json_object = ans.second;
        }
        _msg = json_object.dump(); // json序列化
    }
    else if (_flag == 1)
    { // 文章推荐
        _candicate = _web->doQuery(&tool, _msg, _configure);
        nlohmann::json json_object;
        for (auto &res : _candicate)
        {
            json_object.push_back(res);
        }
        _msg = json_object.dump(); // json序列化
    }
    _con->sendInLoop(_msg);
}

EchoServer::EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port)
    : _pool(threadNum, queSize), _server(ip, port), _configure(new Configuration), _dic(new Dictionary), _web(new WebPageQuery)
{
    /* start(); */
    InputDict(_configure); // 开始将离线库文件读入内存
    nlohmann::json json_object;
}

EchoServer::~EchoServer()
{
    shared_ptr<CacheManager> cacheManager = CacheManager::getInstance();
}

// 服务器的启动与停止
void EchoServer::start()
{
    _pool.start();

    using namespace std::placeholders;
    // 将所有的回调都写到这里来
    _server.setAllCallback(std::bind(&EchoServer::onNewConnection, this, _1) // 占位符后续传tcpconnection对象
                           ,
                           std::bind(&EchoServer::onMessage, this, _1), std::bind(&EchoServer::onClose, this, _1));

    _server.start();
}

void EchoServer::stop()
{
    _pool.stop();
    _server.stop();
}

// 三个回调
void EchoServer::onNewConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!!!" << endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    // 每次要receive的话，要读小火车要三个车厢(int,flag,content)
    int flag;
    string msg = con->receive(flag); // msg是候选词或者要搜索的句子
    cout << ">>>>recv from client : " << msg << endl;

    MyTask task(flag, msg, con, _configure, _dic, _web);
    _pool.addTask(std::bind(&MyTask::process, task, std::placeholders::_1));
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!!!" << endl;
}

void EchoServer::InputDict(const ConfigurationPtr &conf)
{
    _dic->dictInput(conf);
    _web->webInput(conf);
}
