/**
 * Project SearchEngine
 */


#ifndef _REDISSERVER_H
#define _REDISSERVER_H

#include <sw/redis++/redis++.h>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>
using namespace sw::redis;
using std::cout;

class RedisServer {
public:
    static RedisServer *getInstance();

    void set(const std::string& key, const std::string& value);

    std::pair<bool, std::vector<std::string>> get(const std::string& key);

private:
    RedisServer();
    ~RedisServer();
    static RedisServer *instance;
    std::shared_ptr<Redis> _redis;
};

#endif //_REDISSERVER_H