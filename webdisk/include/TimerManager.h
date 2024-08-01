/**
 * Project SearchEngine
 */


#ifndef _TIMERMANAGER_H
#define _TIMERMANAGER_H

#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include "TimerTask.h"
#include "MyTask.h"
#include <mutex>

using namespace std;

class TimerManager {
public: 
    
void start();
    
void stop();
    
/**
 * @param TimerTask*
 */
void attach(TimerTask* task);
void setTimer();
void handleRead();

TimerTask* detach();
private: 
    int _timerfd;
    vector<TimerTask*> _wheelList;
    int _isLoop;
    mutex _mutex;
};

#endif //_TIMERMANAGER_H