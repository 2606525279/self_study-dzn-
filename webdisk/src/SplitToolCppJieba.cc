/**
 * Project SearchEngine
 */


#include "../include/SplitToolCppJieba.h"

/**
 * SplitToolCppJieba implementation
 */


/**
 * @return vector<string>
 */


const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";
    SplitToolCppJieba::SplitToolCppJieba():jb(DICT_PATH,
            HMM_PATH,
            USER_DICT_PATH,
            IDF_PATH,
            STOP_WORD_PATH)
    {
    }
vector<string> SplitToolCppJieba::cut(const string &sentence) {
    
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    vector<pair<string,string>> targes;
    jb.Tag(sentence,targes);
    for(auto &word : targes){
        if(word.second == "eng"){
            words.push_back(word.first);
        }
        if(word.second != "x" && word.second != "eng"){
            words.push_back(word.first);
        }
    }
    return words;
}
