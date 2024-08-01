/**
 * Project SearchEngine
 */


#include "LRUCache.h"




/**
 * @param filename
 * @return void
 */
void LRUCache::readFromFile(const string& filename) {
    ifstream ifs(filename);
    if(!ifs){
        cerr << "ifs open failed\n";
        return ;
    }

    ifs.seekg(0,ios::end);
    if(0 == ifs.tellg()){
        cerr <<"file is null\n";
        return;
    }
    ifs.seekg(0,ios::beg);

    string line;
    string key;
    nlohmann::json json_obj;
    string value;
    istringstream iss(line);
    iss >> key >> value;
    json_obj = nlohmann::json::parse(value);
    _pendingUpdateList.push_back(make_pair(key,json_obj));
    return;
}

pair<bool,nlohmann::json> LRUCache::find(const string &key){
    lock_guard<mutex> lock(_mutex);
    auto it = _hashmap.find(key);
    if(it == _hashmap.end()){
        return (make_pair(false,nullptr));
    }
    return (make_pair(true,*_hashmap[key]));
}
    
void LRUCache::kToit(){
    for(auto it = _resultList.begin() ; it != _resultList.end() ;++it){
        _hashmap[it->first] = it;
    }
    return;
}


void LRUCache::addRecord(const string & key, nlohmann::json &json) {
    lock_guard<mutex> lock(_mutex);
    _pendingUpdateList.push_back(make_pair(key,json));
    return;
}

/**
 * @param const LRUCache&
 * @return void
 */
void LRUCache::update(const LRUCache& cache) {
    lock_guard<mutex> lock(_mutex);
    this->_pendingUpdateList.insert(this->_pendingUpdateList.end(),cache._pendingUpdateList.begin(),cache._pendingUpdateList.end());
    kToit();//每次更新完都要重新映射
    return;
}


void LRUCache::writeToFile(const string& filename) {
    ofstream ofs(filename);
    if(!ofs){
        cerr << "ofs open failed\n";
    }
    for(auto &res : _resultList){
        ofs << res.first << "\t" << res.second <<"\n";
    }
    return;
}