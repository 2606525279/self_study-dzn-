#include "../include/SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
using std::cout;

SocketIO::SocketIO(int fd)
    : _fd(fd)
{
}

SocketIO::~SocketIO()
{
    close(_fd);
}

// len = 10000    1500/6     1000/1
int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *pstr = buf;
    int ret = 0;

    while (left > 0)
    {
        ret = read(_fd, pstr, left);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("read error -1");
            return -1;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
    int left = len - 1;
    char *pstr = buf;
    int ret = 0, total = 0;

    while (left > 0)
    {
        // MSG_PEEK不会将缓冲区中的数据进行清空,只会进行拷贝操作
        ret = recv(_fd, pstr, left, MSG_PEEK);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("readLine error -1");
            return -1;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            for (int idx = 0; idx < ret; ++idx)
            {
                if (pstr[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0'; // C风格字符串以'\0'结尾

                    return total + sz;
                }
            }

            readn(pstr, ret); // 从内核态拷贝到用户态
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

int SocketIO::readTrack(int &flag, char *buf)
{
    int ret;
    int size;
    ret = recv(_fd, &size, sizeof(int), MSG_WAITALL);
    if (-1 == ret)
    {
        perror("readLine error -1");
        return -1;
    }
    ret = recv(_fd, &flag, sizeof(int), MSG_WAITALL);
    if (-1 == ret)
    {
        perror("readLine error -1");
        return -1;
    }
    ret = recv(_fd, buf, size, MSG_WAITALL);
    if (-1 == ret)
    {
        perror("readLine error -1");
        return -1;
    }
    return 0;
}

int SocketIO::writen(const char *buf, int len)
{

    int left = len;
    const char *pstr = buf;
    int ret = 0;

    ret = send(_fd, &len, sizeof(int), MSG_NOSIGNAL);
    if (-1 == ret)
    {
        perror("writen error -1");
        return -1;
    }

    while (left > 0)
    {
        ret = write(_fd, pstr, left);
        if (-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if (-1 == ret)
        {
            perror("writen error -1");
            return -1;
        }
        else if (0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}
