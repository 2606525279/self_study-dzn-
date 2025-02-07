#include "../include/TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port)
: _acceptor(ip, port)
, _loop(_acceptor)
{

}

TcpServer::~TcpServer()
{

}

//服务器启动与停止
void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

//将三个回调一次都注册进来
void TcpServer::setAllCallback(TcpConnectionCallback &&cb1
                              , TcpConnectionCallback &&cb2
                              , TcpConnectionCallback &&cb3)
{
    _loop.setNewConnectionCallback(std::move(cb1));
    _loop.setMessageCalback(std::move(cb2));
    _loop.setCloseCallback(std::move(cb3));
}
