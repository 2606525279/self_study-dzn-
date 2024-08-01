/**
 * Project SearchEngine
 */


#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <memory>
#include <cmath>
#include <sstream>
#include <math.h>
#include <iostream>
#include <algorithm> // 为了使用 std::transform
#include <cctype> // 为了使用 std::tolower
#include <dirent.h>
#include "tinyxml2.h"
#include "SplitTool.h"




using namespace std;
using namespace tinyxml2;

class DictProducer {
public: 
    
/**
 * @param string
 * @param tool
 */
DictProducer(const string &dir, SplitTool* tool);//ch
DictProducer(const string &dir);//en
DictProducer(){} 
void buildEnDict();
    
void buildCnDict();
    
void createIndex(ofstream &ofs_dict,ofstream &ofs_index);

void stop_input(const string &filepath);

void store();
private: 
    vector<string> _files;//文件名
    vector<string> _jieba;
    vector<pair<string,int>> _dict;//词典
    map<string,int> _freq;
    map<string,set<int>> _index;//index
    SplitTool* _cuttor;//分离工具
    map<string,int> _stop;
};


class InvertIndex{
public:
    InvertIndex(SplitTool *tool)
    :_currtor(tool)
    {
        
    }

    void start(const string &newripepage,const string &newoffset,const string&enstop,const string& zhstop,const string& invertPath);
    void stopnew(const string &filepath);
    string read(const string &content);//将html标签提取出来
    void countPri();//tf/idf
    void store(const string& invertPath);

private:
    SplitTool *_currtor;
    unordered_map<string,set<pair<int,double>>> _invert;//word:<pageindex,priority>
    map<string,int> _stop;
    map<string,set<int>> _countPage;//包含该词的所有文章数量
    map<pair<string,int>,int> _count;//某文章中包含该单词的数量
    map<int,pair<size_t,size_t>> _index;//网页坐标
    map<int,set<string>> _ptow;//文章对应单词
    map<string,double> _wn;//这篇文章的权重，后面平方和用
    int _num;//文档总数
};


#endif //_DICTPRODUCER_H
