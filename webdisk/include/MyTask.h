/**
 * Project SearchEngine
 */


#ifndef _MYTASK_H
#define _MYTASK_H

#include "TimerTask.h"


class MyTask: public TimerTask {
public: 
    
void process() override;
};

#endif //_MYTASK_H