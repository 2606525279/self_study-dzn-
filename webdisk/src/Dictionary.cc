/**
 * Project SearchEngine
 */

#include "../include/Dictionary.h"

/**
 * Dictionary implementation
 */

/**
 * @param key
 * @return vector<string>
 */
vector<string> Dictionary::doQuery(const ConfigurationPtr &conf,const string &key)
{
    set<int> unionEn;
    set<int> unionCh;
    string en; // 英文子串
    string zh; // 中文子串
    int Num = 1;
    vector<string> result;
    //并集
    for (size_t i = 0; i < key.size(); ) {
    if ((0x80 & key[i]) == 0) { // eng
        unionEn.insert((_enindex[string(1, key[i])]).begin(), (_enindex[string(1, key[i])]).end());
        ++i;
    } else { // ch
        if (i + 2 < key.size()) {
            std::string ch = key.substr(i, 3); // Extract the 3-byte Chinese character
            unionCh.insert((_chindex[ch]).begin(), (_chindex[ch]).end());
            i += 3; // Move to the next character
        } else {
            // Handle the error case where a Chinese character is not fully represented
            break;
        }
    }
    }

    if ((0x80 & key[0]) == 0){ // key是英文，对英文做处理  
        for(auto it = unionEn.begin() ; it != unionEn.end();++it){
            istringstream iss((_endict[*it]));;
            string word;
            int freq;
            iss >> word >> freq;
            _freq[word] = freq;//并集
        }

    }else{// key是中文，对中文做处理
        for(auto it = unionCh.begin() ; it != unionCh.end();++it){
            istringstream iss((_chdict[*it]));
            string word;
            int freq;
            iss >> word >> freq;
            _freq[word] = freq;//并集
        }
    }

    for(auto &res:_freq){
        int dis = distance(key,res.first);
        _candidateResult.word = res.first;
        _candidateResult.freq = res.second;
        _candidateResult.distance = dis;
        _queue.push(_candidateResult);
    }
    while(Num <= 5  && !_queue.empty()){
        _dict.push_back(_queue.top().word);
        ++Num;
        _queue.pop();
    }
    _queue = std::priority_queue<candidateResult,vector<candidateResult>,MyLess>();
    result.swap(_dict);
    return result;
}

void Dictionary::queryIndex()
{
    return;
}

size_t nBytesCode(const char ch)
{
    if (ch & (1 << 7))
    {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx)
        {
            if (ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
                break;
        }
        return nBytes;
    }
    return 1;
}

std::size_t length(const std::string &str)
{
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int Dictionary::distance(const string &lhs, const string &rhs)
{
    // 计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for (size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;
    }
    for (size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;
    }
    std::string sublhs, subrhs;
    for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i,
                     ++lhs_idx)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);
        for (std::size_t dist_j = 1, rhs_idx = 0;
             dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if (sublhs == subrhs)
            {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j -
                                                                1];
            }
            else
            {
                editDist[dist_i][dist_j] =
                    triple_min(editDist[dist_i][dist_j - 1] + 1,
                               editDist[dist_i - 1][dist_j] + 1,
                               editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }
    return editDist[lhs_len][rhs_len];
    return 0;
}

void Dictionary::chInput(const ConfigurationPtr &conf){
    ifstream ifs(conf->getCnDict());
    if (!ifs)
    {
        cerr << "ifs error \n";
    }
    string line;
    while (getline(ifs, line))
    {
        _chdict.push_back(line);
        
    }
    ifs.close();
}
void Dictionary::enInput(const ConfigurationPtr &conf){
    ifstream ifs(conf->getEnDict());
    if (!ifs)
    {
        cerr << "ifs error \n";
    }
    string line;
    while (getline(ifs, line))
    {
        _endict.push_back(line);
        
    }
    ifs.close();
}
void Dictionary::chIndexInput(const ConfigurationPtr &conf)
{
    ifstream ifs(conf->getCnIndexDict());
    if (!ifs)
    {
        cerr << "ifs error \n";
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word;
        int index;
        iss >> word;
        while (iss >> index)
        {
            _chindex[word].insert(index);
        }
    }
    ifs.close();
}

void Dictionary::enIndexInput(const ConfigurationPtr &conf)
{
    ifstream ifs(conf->getEnIndexDict());
    if (!ifs)
    {
        cerr << "ifs error \n";
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word;
        int index;
        iss >> word;
        while (iss >> index)
        {
            _enindex[word].insert(index);
        }
    }
    ifs.close();
}

void Dictionary::dictInput(const ConfigurationPtr &conf)
{
    chInput(conf);
    enInput(conf);
    chIndexInput(conf);
    enIndexInput(conf);
}