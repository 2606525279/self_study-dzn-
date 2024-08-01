/**
 * Project SearchEngine
 */

#ifndef _DICTIONARY_H
#define _DICTIONARY_H
#include <string>
#include <vector>
#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <iostream>

struct candidateResult{
    string word;
    int freq;
    int distance;
};

struct MyLess{
    bool operator()(const candidateResult &a,const candidateResult &b){
        if(a.distance != b.distance){
            return a.distance > b.distance;
        }else{
            return a.freq < b.freq;
        }
    }
};


using namespace std;
using ConfigurationPtr = shared_ptr<Configuration>;


class Dictionary
:public std::enable_shared_from_this<Dictionary>
{
public:
    using DictionaryPtr = shared_ptr<Dictionary>;
    Dictionary(){}
    vector<string> doQuery(const ConfigurationPtr &conf,const string &key);
    void dictInput(const ConfigurationPtr &conf);
    void chInput(const ConfigurationPtr &conf);
    void enInput(const ConfigurationPtr &conf);
    void chIndexInput(const ConfigurationPtr &conf);
    void enIndexInput(const ConfigurationPtr &conf);
    

private:
    vector<string> _chdict; // 中文词频
    vector<string> _endict; // 英文词频
    vector<string> _dict;//候选词列表
    map<string, set<int>> _enindex;    // 坐标
    map<string, set<int>> _chindex;    // 坐标
    candidateResult _candidateResult;//单词，频率，最小编辑距离
    priority_queue<candidateResult,vector<candidateResult>,MyLess> _queue;//优先级队列.升序
    map<string,int> _freq;//词频,也是并集
    void queryIndex();

    int distance(const string &lhs,const string &rhs);//最小编辑算法
};

#endif //_DICTIONARY_H