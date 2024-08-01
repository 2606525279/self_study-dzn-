/**
 * Project SearchEngine
 */

#include "../include/TimerManager.h"
#include <thread>
#include <poll.h>

TimerManager::TimerManager()
{
    _timerfd = timerfd_create(CLOCK_REALTIME, 0);
    if (_timerfd < 0)
    {
        cerr << "createTimerFd\n";
        return;
    }
}

void TimerManager::setTimer()
{
    itimerspec newValue;
    newValue.it_interval.tv_sec = 2;
    newValue.it_interval.tv_nsec = 0;

    newValue.it_value.tv_sec = 2;
    newValue.it_value.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &newValue, 0);
    if (ret < 0)
    {
        cerr << "setTimerFd\n";
        return;
    }
}

void TimerManager::handleRead()
{
    uint64_t one = 1;
    ssize_t ret = read(_timerfd, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t))
    {
        perror("handleRead");
        return;
    }
}



void TimerManager::start()
{
    struct pollfd pfd;
    pfd.fd = _timerfd;
    pfd.events = POLLIN;
    setTimer();
    _isLoop = true;
    while(_isLoop){
        int nready = poll(&pfd, 1, 5000);
        if(-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            cerr << "-1 == nready" << endl;
            return;
        }
        else 
        {
            if(pfd.revents & POLLIN)
            {
                handleRead();
                MyTask task;
                attach(&task);
            }
        }
    
    }
    return;
}

void TimerManager::stop()
{
    if (_isLoop)
    {
        _isLoop = false;
        itimerspec newValue;
        newValue.it_interval.tv_sec = 0;
        newValue.it_interval.tv_nsec = 0;

        newValue.it_value.tv_sec = 0;
        newValue.it_value.tv_nsec = 0;

        int ret = timerfd_settime(_timerfd, 0, &newValue, 0);
        if (ret < 0)
        {
            cerr << "setTimerFd\n";
            return;
        }
    }
    return;
}

void TimerManager::attach(TimerTask *task)
{
    lock_guard<mutex> lock(_mutex);
    _wheelList.push_back(task);
    return;
}

TimerTask* TimerManager::detach()
{
    while(_wheelList.size() == 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    lock_guard<mutex> lock(_mutex);
    TimerTask *task = _wheelList.back();
    _wheelList.pop_back();
    return;
}