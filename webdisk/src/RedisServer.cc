/**
 * Project SearchEngine
 */


#include "../include/RedisServer.h"

RedisServer* RedisServer::instance = nullptr;

RedisServer::RedisServer():_redis(std::make_shared<Redis>("tcp://127.0.0.1:6379")) {}

RedisServer *RedisServer::getInstance(){
        if(instance == nullptr){
            instance = new RedisServer();
        }
        return instance;
    }

void RedisServer::set(const std::string& key, const std::string& value) {
        
        _redis->rpush(key, value);
    }

std::pair<bool, std::vector<std::string>> RedisServer::get(const std::string& key) {
        cout << "redis get\n";
        std::vector<std::string> vec;
        _redis->lrange(key, 0, -1, std::back_inserter(vec));
        if (vec.empty()) {
            return {false, {}};
        }
        return {true, vec};//前一个值来判断值是否存在
    }

/**
 * RedisServer implementation
 */
