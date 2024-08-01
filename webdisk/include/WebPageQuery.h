/**
 * Project SearchEngine
 */

#ifndef _WEBPAGEQUERY_H
#define _WEBPAGEQUERY_H
#include <string>
#include <vector>
#include <unordered_map>
#include "Configuration.h"
#include "SplitTool.h"
#include "RedisServer.h"

#include "json.hpp"
#include <fstream>
#include <queue>
#include <sstream>
#include <set>
#include <cmath>
#include <math.h>
#include <iostream>
#include <map>


using namespace std;
using ConfigurationPtr = shared_ptr<Configuration>;


struct Comp{
    string word;
    int idx;
    double cos;
};

struct MyOptLess{
    bool operator()(const Comp &a,const Comp &b){
        return a.cos < b.cos;
    }
};

class WebPageQuery
:public std::enable_shared_from_this<WebPageQuery>
{
public:
    using WebPageQueryPtr = shared_ptr<WebPageQuery>;
    WebPageQuery():_redis(RedisServer::getInstance()){}
    

    vector<string> doQuery(SplitTool *tool,string key,const ConfigurationPtr &conf);

    void countTf();
    void baseLine();
    void stopInput(const string &filepath);
    void offsetInput(const ConfigurationPtr &conf);
    void invertInput(const ConfigurationPtr &conf);
    void webInput(const ConfigurationPtr &conf);

private:
    // vector<WebPage> _pages;

    map<int, pair<size_t, size_t>> _offsetlib;      // 网页偏移库
    unordered_map<string, set<pair<int, double>>> _invertIndexLib; // 倒排索引库,有每个单词在哪篇文章，以及权重
    map<string,int> _stop;
    map<string,int> _search;//句子中该单词的词频
    map<string,double> _winput;//句子中该单词的权重
    priority_queue<Comp,vector<Comp>,MyOptLess> _queue;
    RedisServer *_redis;
};

#endif //_WEBPAGEQUERY_H