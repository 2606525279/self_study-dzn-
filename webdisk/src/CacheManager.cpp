/**
 * Project SearchEngine
 */

#include "CacheManager.h"

shared_ptr<CacheManager> CacheManager::instance = nullptr;



CacheManager::~CacheManager()
{
    for (auto &res : _caches)
    {
        res.writeToFile("/home/dzn/Linux/linux57/webdisk/data/cache.dat");
    }
}

void CacheManager::createNewCache(const string &filename, int idx)
{
    LRUCache cache;
    _caches.push_back(cache);
    init(filename,_caches[idx]);
}

void CacheManager::init(const string &filename, LRUCache &cache)
{

    cache.readFromFile(filename);  
    return;
}

LRUCache &CacheManager::getCache(int index)
{
    if(_caches.size() < (index + 1)){
        cerr << "_caches capacity not enough\n";
        return;
    }
    return _caches[index];
}

void CacheManager::updateCache()
{
    for (int idx = 0; idx < _caches.size() - 1; ++idx)
    {
        _caches[_caches.size()].update(_caches[idx]);
    }
    return;
}