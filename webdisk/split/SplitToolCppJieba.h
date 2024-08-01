/**
 * Project SearchEngine
 */


#ifndef _SPLITTOOLCPPJIEBA_H
#define _SPLITTOOLCPPJIEBA_H

#include "SplitTool.h"

class SplitToolCppJieba: public SplitTool {
public: 
    SplitToolCppJieba();
    vector<string> cut(const string &sentence);
private:
    cppjieba::Jieba jb;
};

#endif //_SPLITTOOLCPPJIEBA_H