/**
 * Project SearchEngine
 */


#ifndef _SPLITTOOL_H
#define _SPLITTOOL_H

#include <string>
#include <vector>
#include "cppjieba/Jieba.hpp"
using namespace std;

class SplitTool {
public:   
    virtual vector<string> cut(const string &sentence) = 0;
};

#endif //_SPLITTOOL_H