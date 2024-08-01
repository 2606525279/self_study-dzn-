#include "tinyxml2.h"
#include <iostream>
#include <string>
#include "simhash/Simhasher.hpp"
#include <vector>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
#define LOGGER_LEVEL LL_WARN
using namespace std;
using namespace tinyxml2;
using namespace simhash;

struct RSSIteam
{
    RSSIteam() = default;
    string _title;
    string _link;
    string _description;
    // string _content;
};

class RSS
{
public:
    RSS(const string &jiebaDict, const string &hmmModel, const string &idf, const string &stopWords)
        : simhasher(jiebaDict, hmmModel, idf, stopWords)
    {

        _size = 0;
        _start = 0;
    }
    void start(const string &dir, const string outpath, const string &offsetpath, const string &newoffset, const string &newwripe);
    void store(const string &outpath);
    void read(const string &outpath);
    void store_off(const string &offsetpath);
    uint64_t calsimhash(const string &content);
    bool isSimilar(uint64_t hash1, uint64_t hash2);

private:
    Simhasher simhasher;
    size_t _size;
    size_t _start;
    vector<string> _files;
    vector<RSSIteam> _rss;
    vector<pair<size_t, size_t>> _offset;
    map<int, uint64_t> _simhash; //(start,hash)
    map<int, pair<size_t, size_t>> _webIdx;
};

void RSS::start(const string &dir, const string outpath, const string &offsetpath, const string &newoffset, const string &newwripe)
{
    // 读入xml
    DIR *dir_ptr;
    struct dirent *ent;
    string dir_path;
    int count = 1;
    if ((dir_ptr = opendir(dir.c_str())) != nullptr)
    {
        while ((ent = readdir(dir_ptr)) != nullptr)
        {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            {
                continue;
            }
            dir_path = dir + "/" + string(ent->d_name);
            _files.push_back(dir_path);
        }
    }

    int id = 1;
    for (auto &res : _files)
    {
        read(res);
    }
    store(outpath);
    store_off(offsetpath);
}

void RSS::read(const string &filename)
{
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    if (doc.ErrorID())
    {
        std::cerr << "loadFile fail" << endl;
        return;
    }
    size_t idx = 0;
    XMLElement *itemNode =
        doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    while (itemNode)
    {
        string title = itemNode->FirstChildElement("title")->GetText();
        string link = itemNode->FirstChildElement("link")->GetText();
        string description = "";
        string content = "";
        if (itemNode->FirstChildElement("description"))
        {
            description = itemNode->FirstChildElement("description")->GetText();
        }
        // if (itemNode->FirstChildElement("content"))
        // {
        //     description += itemNode->FirstChildElement("content")->GetText();
        // }
        // if (itemNode->FirstChildElement("content:encoded"))
        // {
        //     description += itemNode->FirstChildElement("content:encoded")->GetText();
        // }
        std::regex reg("<[^>]+>"); // 通用正则表达式
        description = regex_replace(description, reg, "");
        title = regex_replace(title, reg, "");
        description.erase(std::remove_if(description.begin(), description.end(), [](char c) { return c == '\n'; }), description.end());
        RSSIteam rssItem;

        rssItem._title = title;
        rssItem._link = link;
        rssItem._description = description;
        // rssItem._content = content;

        _rss.push_back(rssItem);
        ++idx;

        itemNode = itemNode->NextSiblingElement("item");
    }
}

void RSS::store(const string &outpath)
{
    int count = 1;
    ofstream ofs(outpath, std::ios::app);
    string temp;
    if (!ofs)
    {
        std::cerr << "open " << outpath << " fail!" << endl;
        return;
    }
    for (size_t idx = 0; idx < _rss.size(); ++idx)
    {
        

        // hash
        // string hash_content = _rss[idx]._description + _rss[idx]._content;
        uint64_t hash = calsimhash(_rss[idx]._description);
        
        bool similar = false;
        // 检查是否有和之前的有相似文章

        for(auto it = _simhash.begin() ; it != _simhash.end();++it){
            similar = isSimilar(hash, (*it).second);
            if (similar == true)
            { // 相似去重,这篇不放入
                break;
            }
        }
        if(similar){
            similar = false;
            continue;
        }
        // 全都不相似，写入
        ostringstream os;
        // os << "<doc>\n\t<docid>" << count++
        //    << "</docid>\n\t<title>" << _rss[idx]._title
        //    << "</title>\n\t<link>" << _rss[idx]._link
        //    << "</link>\n\t<description>" << _rss[idx]._description
        //    << "</description>\n\t<content>" << _rss[idx]._content << "</content>\n</doc>";
        // os << '\n';
        os << "<doc>\n\t<docid>" << count++
           << "</docid>\n\t<title>" << _rss[idx]._title
           << "</title>\n\t<link>" << _rss[idx]._link
           << "</link>\n\t<description>" << _rss[idx]._description
           << "</description>\n</doc>";
        os << '\n';

        temp = os.str();
        ofs << temp;
        _size = temp.size();
        _offset.push_back(make_pair(_start, _size));
        _webIdx[_offset.size() - 1] = make_pair(_start, _size);

        _start += _size;
        _simhash[idx] = hash;
    }

    ofs.close();
}

void RSS::store_off(const string &offsetpath)
{
    ofstream ofs(offsetpath);
    int idx = 1;
    if (!ofs)
    {
        std::cerr << "open " << offsetpath << " fail!" << endl;
        return;
    }

    for (auto &res : _webIdx) // first->start,second->size
    {
        ofs << res.first << "\t" << res.second.first << "\t" << res.second.second << "\n";
    }
    ofs.close();
}

uint64_t RSS::calsimhash(const string &content)
{
    size_t topN = 5;
    uint64_t u1 = 0; // hash
    simhasher.make(content, topN, u1);
    return u1;
}

bool RSS::isSimilar(uint64_t hash1, uint64_t hash2)
{
    return Simhasher::isEqual(hash1, hash2); // 默认3,如果是true则相似，去重
}

int main()
{
    RSS rss("./submodules/cppjieba/dict/jieba.dict.utf8",
            "./submodules/cppjieba/dict/hmm_model.utf8",
            "./submodules/cppjieba/dict/idf.utf8",
            "./submodules/cppjieba/dict/stop_words.utf8");

    rss.start("/home/dzn/Linux/linux57/webdisk/split/yuliao/charticle",
              "/home/dzn/Linux/linux57/webdisk/data/newripepage.dat",
              "/home/dzn/Linux/linux57/webdisk/data/newoffset.dat",
              "/home/dzn/Linux/linux57/webdisk/data/offsetnew.dat",
              "/home/dzn/Linux/linux57/webdisk/data/wripepagenew.dat");
    return 0;
}
