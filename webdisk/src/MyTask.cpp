/**
 * Project SearchEngine
 */


#include "MyTask.h"

/**
 * MyTask implementation
 */


/**
 * @return void
 */
void MyTask::process() {
   shared_ptr<CacheManager> cacheManager = CacheManager::getInstance();
    cacheManager->updateCache();
    return;
}