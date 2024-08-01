/**
 * Project SearchEngine
 */


#ifndef _TIMERTASK_H
#define _TIMERTASK_H

#include "CacheManager.h"

class TimerTask {
public: 
    
virtual void process() = 0;
};

#endif //_TIMERTASK_H