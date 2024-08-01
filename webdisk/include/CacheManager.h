/**
 * Project SearchEngine
 */


#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

#include <string>
#include "LRUCache.h"
#include <vector>
#include <iostream>
#include "json.hpp"
#include <memory>

using std::vector;
using std::string;
using namespace std;

class CacheManager {
public: 

static std::shared_ptr<CacheManager> getInstance(){
    if(instance == nullptr){
        instance = std::make_shared<CacheManager>();
    }
    return instance;
}

void init(const string &filename, LRUCache &cache);
    
/**
 * @param int
 */
void createNewCache(const string &filename,int idx);

LRUCache& getCache(int index);//通过下标返回某个cache
    
void updateCache();
private: 
    CacheManager();
    ~CacheManager();
    static std::shared_ptr<CacheManager> instance;
    vector<LRUCache> _caches;
};

#endif //_CACHEMANAGER_H