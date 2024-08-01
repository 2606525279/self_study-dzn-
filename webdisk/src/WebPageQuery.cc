/**
 * Project SearchEngine
 */

#include "../include/WebPageQuery.h"
#include "../include/tinyxml2.h"

using namespace tinyxml2;


vector<string> Xml(const string &msg){
    vector<string> ans;
    XMLDocument doc;
    string result;
    string description2;
    string title2;
    string docid2;
    string link2;
    const char *xml = msg.c_str();
    doc.Parse(xml);


    XMLElement *docidElement = doc.FirstChildElement("doc")->FirstChildElement("docid");
    const char *docid1 = docidElement->GetText();
    if(docid1 != nullptr){
        string docid(docid1);
        docid2 = docid;
    }

    ans.push_back(docid2);

    XMLElement *titleElement = doc.FirstChildElement("doc")->FirstChildElement("title");
    const char *title1 = titleElement->GetText();
    if(title1 != nullptr){
        string title(title1);
        title2 = title;
    }

    ans.push_back(title2);

    XMLElement *linkElement = doc.FirstChildElement("doc")->FirstChildElement("link");
    const char *link1 = linkElement->GetText();
    if(link1 != nullptr){
        string link(link1);
        link2 = link;
    }
    ans.push_back(link2);

    return ans;
}

void WebPageQuery::countTf(){
    double sum = 0;
    for(auto &res : _search){
        sum += res.second;
    }
    for(auto &res :_search){
        _winput[res.first] = (res.second) / sum;
    }
}

void WebPageQuery::baseLine(){
    double sum = 0;//X * Y
    int num = _winput.size();
    Comp _com;
    double x_sqrt = 0;//所有关键词(x)的平方和的开根号
    double y_sqrt = 0;

    for(auto &res : _winput){
        sum += res.second * res.second;
    }
    x_sqrt = std::sqrt(sum);
    sum = 0;

    map<int,int> search;//index->count,如果某个网页下标数目命中次数大于等于单词总数，则为公共网页
    unordered_map<int,set<pair<string,double>>> _intersection;//该公共网页对应的所有关键词的权重

    for(auto &res : _winput){//所有分割出来的单词
        auto it = _invertIndexLib.find(res.first);//准备遍历该单词对应的所有网页
        if(it == _invertIndexLib.end()){
            cerr << "error find such web\n";
            return;
        }
        for(auto idx = (it->second).begin() ; idx != (it->second).end();++idx){//遍历set
            ++search[idx->first];
        }
    }

    for(auto &res : search){//找到所有公共网页的所有关键词的权重
        if(res.second >= num){//公共网页
            for(auto &word : _winput){//单词
                for(auto it = _invertIndexLib[word.first].begin(); it != _invertIndexLib[word.first].end();++it){
                    if(it->first == res.first){
                        _intersection[res.first].insert(make_pair(word.first,it->second));
                    }
                }
            }
        }
    }

    //计算基准向量
    for(auto &res : _intersection){
        double union_y = 0;
        for(auto it = res.second.begin() ; it != res.second.end() ;++it){
            sum += _winput[it->first] * (it->second);//单词在句子中的权重 * 单词在这篇文章中的权重
        
            _com.idx = res.first;
            _com.word = it->first;
            union_y += (it->second) *(it->second);//y1*y1 + y2*y2 + y3*y3
        }
        y_sqrt = std::sqrt(union_y);
        _com.cos = (sum) / (x_sqrt * y_sqrt);
        _queue.push(_com);
        sum = 0;
    }
}

vector<string> WebPageQuery::doQuery(SplitTool *tool,string key,const ConfigurationPtr &conf)
{
    auto redisResult = _redis->get(key);
    if(redisResult.first){
        cout << "Redis found\n";
        return redisResult.second;
    }

    cout << "Redis not found\n";

    ifstream ifs(conf->getNewipepage());
    if(!ifs){
        cerr << "ifs error\n";
        return {};
    }
    
    vector<string> result;
    int Num =1;
    result = tool->cut(key);

    cout << "result : " << result << "\n";

    for(auto &res : result){
        if(_stop[res] < 1){
            ++_search[res];
        }
    }

    countTf();//算出用户输入的权重
    baseLine();

    result = vector<string>();//清空一下

    while(Num <= 10 && !_queue.empty()){//找出前十个最符合的网页
        Comp comp = _queue.top();
        _queue.pop();
        size_t start;
        size_t size;
        start = _offsetlib[comp.idx].first;
        size = _offsetlib[comp.idx].second;
        string content;
        content.resize(size);
        ifs.seekg(start,ios::beg);
        ifs.read(content.data(),size);
        vector<string> ans = Xml(content);
        result.insert(result.end(),ans.begin(),ans.end());
        // result.push_back(content);
    }

    //redis->set
    for(auto &res : _winput){//对每个单词都建立缓存
        for(auto &ans : result){
            _redis->set(res.first,ans);
        }
    }

    _queue = std::priority_queue<Comp,vector<Comp>,MyOptLess>();
    _search.clear();
    _winput.clear();
    return result;
}


void WebPageQuery::stopInput(const string &filepath){
    ifstream ifs(filepath);
    if (!ifs)
    {
        cerr << "ifs error stop\n";
    }
    string word;
    while (ifs >> word)
    {
        if (0x80 & word[0] == 0)
        { // 英文
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                       { return std::tolower((c)); });
        }
        ++_stop[word];
    }
    ifs.close();
}
void WebPageQuery::offsetInput(const ConfigurationPtr &conf)
{
    ifstream ifs(conf->getNewOffset());
    if (!ifs)
    {
        cerr << "ifs error stop\n";
    }
    string line;
    while(getline(ifs,line)){
        istringstream iss(line);
        int index;
        size_t start,size;
        iss >> index >> start >> size;
        _offsetlib[index] = make_pair(start,size);
    }
    ifs.close();
}
void WebPageQuery::invertInput(const ConfigurationPtr &conf)
{
    ifstream ifs(conf->getInvertPath());
    if (!ifs)
    {
        cerr << "ifs error stop\n";
    }
    string line;
    while(getline(ifs,line)){
        istringstream iss(line);
        string word;
        int index;
        double pri;
        iss >> word >> index >> pri;
        _invertIndexLib[word].insert(make_pair(index,pri));
    }
    ifs.close();
}
void WebPageQuery::webInput(const ConfigurationPtr &conf)
{
    stopInput("/home/dzn/Linux/linux57/webdisk/split/yuliao/enstop/stop_words_eng.txt");
    stopInput("/home/dzn/Linux/linux57/webdisk/split/yuliao/stop/stop_words_zh.txt");
    offsetInput(conf);
    invertInput(conf);
}
