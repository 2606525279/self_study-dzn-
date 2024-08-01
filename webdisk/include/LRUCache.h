/**
 * Project SearchEngine
 */


#ifndef _LRUCACHE_H
#define _LRUCACHE_H
#include <string>
#include <unordered_map>
#include <list>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <mutex>
#include <iostream>


using namespace std;


class LRUCache {
public: 
    
/**
 * @param filename
 */
void readFromFile(const string& filename);
    
/**
 * @param key
 * @param json
 */
void addRecord(const string & key, nlohmann::json &json);

pair<bool,nlohmann::json> find(const string &key);
    
void kToit();

void update(const LRUCache& cache);
    

void writeToFile(const string& filename);//持久化
private: 
    list<pair<string,nlohmann::json>> _resultList;
    unordered_map<string,list<pair<string,nlohmann::json>>::iterator> _hashmap;// key -> pair
    list<pair<string,nlohmann::json>> _pendingUpdateList; 
    mutex _mutex;
};

#endif //_LRUCACHE_H