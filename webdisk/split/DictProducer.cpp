/**
 * Project SearchEngine
 */

#include "DictProducer.h"
/**
 * DictProducer implementation
 */

/**
 * @param string
 * @param tool
 */

void dealword(string &words)
{
    // for (int idx = 0; idx != words.size(); ++idx)
    // {
        // words.erase(std::remove_if(words.begin(), words.end(), [](char c)
        //                            { return !std::isalpha(static_cast<unsigned char>(c)); }),
        //             words.end());
        std::transform(words.begin(), words.end(), words.begin(), [](char c) {
                return (std::isalpha(c) ? c : ' ');
         });
        
        
        // Convert all characters to lowercase
        std::transform(words.begin(), words.end(), words.begin(), [](char c)
                       { return std::tolower((c)); });
        return;
    // }
}

void DictProducer::stop_input(const string &filepath)
{
    ifstream ifs(filepath);
    if (!ifs)
    {
        cerr << "ifs error stop\n";
    }
    // string line;

    // while (getline(ifs, line))
    // {
    //     istringstream is(line);
    //     string word;
    //     while (is >> word)
    //     {
    //         if(0x80 & word[0] == 0){//英文
    //             dealword(word);
    //         }
    //         ++_stop[word];
    //     }
    string word;
    while (ifs >> word)
    {
        if (0x80 & word[0] == 0)
        { // 英文
            dealword(word);
        }
        ++_stop[word];
    }
    ifs.close();
}

DictProducer::DictProducer(const string &dir, SplitTool *tool)
    : _cuttor(tool)
{
    DIR *dir_ptr;
    struct dirent *ent;
    string dir_path;
    stop_input("/home/dzn/Linux/linux57/webdisk/split/yuliao/stop/stop_words_zh.txt");
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
            cout << dir_path << "\n";
        }
    }

} // ch

DictProducer::DictProducer(const string &dir)
{
    DIR *dir_ptr;
    struct dirent *ent;
    string dir_path;
    stop_input("/home/dzn/Linux/linux57/webdisk/split/yuliao/enstop/stop_words_eng.txt");
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
            cout << dir_path << "\n";
        }
    }
} // en

/**
 * @return void
 */
void DictProducer::buildEnDict()
{
    ofstream ofs_index("/home/dzn/Linux/linux57/webdisk/data/endictIndex.dat", std::ios::app);
    if (!ofs_index)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
    ofstream ofs_dict("/home/dzn/Linux/linux57/webdisk/data/endict.dat", std::ios::app);
    if (!ofs_dict)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
    //  对每个文件进行分词
    for (auto _filepath : _files)
    {
        ifstream ifs(_filepath);
        if (!ifs)
        {
            cerr << "en ifs error\n";
        }
        ifs.seekg(0, std::ios::end);
        int fileLength = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        char *buff = new char[fileLength + 1];
        ifs.read(buff, fileLength);
        string contents = buff;
        for(auto &res : contents){
            if(ispunct(res)){
                res = ' ';
            }
            if(isdigit(res)){
                res = ' ';
            }
        }
        istringstream is(contents);
        string words;
        while (is >> words)
        {
            dealword(words);
            if (words == "")
            {
                continue;
            }
            if (_stop[words] < 1)
            {
                ++_freq[words];
            }
            else
            {
                continue;
            }
        }
        ifs.close();
    }

    createIndex(ofs_dict, ofs_index);
    ofs_dict.close();
    ofs_index.close();
    return;
}

/**
 * @return void
 */
void DictProducer::buildCnDict()
{
    ofstream ofs_index("/home/dzn/Linux/linux57/webdisk/data/chdictIndex.dat", std::ios::app);
    if (!ofs_index)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
    ofstream ofs_dict("/home/dzn/Linux/linux57/webdisk/data/chdict.dat", std::ios::app);
    if (!ofs_dict)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
    //  对每个文件进行分词
    for (auto _filepath : _files)
    {
        ifstream ifs(_filepath);
        if (!ifs)
        {
            cerr << "en ifs error\n";
        }
        ifs.seekg(0, std::ios::end);
        int fileLength = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        char buff[fileLength + 1];
        memset(buff, 0, sizeof(buff));
        ifs.read(buff, fileLength);
        string contents = buff;
        contents.erase(std::remove_if(contents.begin(), contents.end(), [](char c)
                                      { return c == '\r' || c == '\n'; }),
                       contents.end());

        _jieba = _cuttor->cut(contents); // 对每篇中文就行jieba
        for (auto &res : _jieba)
        {
            if (!_stop.count(res) && ((0x80 & res[0]) != 0))
            {
                ++_freq[res];
            }
            else
            {
                continue;
            }
        }

        ifs.close();
    }
    createIndex(ofs_dict, ofs_index);
    ofs_dict.close();
    ofs_index.close();
    return;
}

/**
 * @return void
 */
void DictProducer::createIndex(ofstream &ofs_dict, ofstream &ofs_index)
{
    // 分离完所有文件后，将单词开始写入到dat文件中
    for (auto res : _freq)
    {
        if ((0x80 & res.first[0]) == 0) // 英文
        {
            _dict.push_back(res);
            ofs_dict << res.first << "\t" << res.second << "\n"; // 词频文件
            for (int idx = 0; idx != res.first.size(); ++idx)
            {
                _index[string(1, res.first[idx])].insert(_dict.size() - 1);
            }
        }
        else
        { // 汉字
            _dict.push_back(res);
            ofs_dict << res.first << "\t" << res.second << "\n"; // 词频文件
            for (int idx = 0; idx < res.first.size(); idx += 3)
            {
                _index[res.first.substr(idx, 3)].insert(_dict.size() - 1);
            }
        }
    }
    for (auto idx : _index)
    { // 索引文件
        if ((0x80 & idx.first[0]) == 0)
        {
            ofs_index << idx.first << "\t";
            for (auto it = idx.second.begin(); it != idx.second.end(); ++it)
            {
                ofs_index << *it;
                if (std::next(it) != idx.second.end())
                {
                    ofs_index << "\t";
                }
            }
            ofs_index << "\n";
        }
        else
        {
            ofs_index << idx.first << "\t";
            for (auto it = idx.second.begin(); it != idx.second.end(); ++it)
            {
                ofs_index << *it;
                if (std::next(it) != idx.second.end())
                {
                    ofs_index << "\t";
                }
            }
            ofs_index << "\n";
        }
    }
    return;
}

/**
 * @return void
 */
void DictProducer::store()
{
    return;
}

// void start(const string &newripepage,const string &newoffset);
// void countPage();//统计网页数量
// void stopInput(const string &filepath);
// void read(const string &filepath);
// void countPri();//tf/idf

string InvertIndex::read(const string &content)
{
    XMLDocument doc;
    string result;
    string description2;
    string title2;
    const char *xml = content.c_str();
    doc.Parse(xml);
    XMLElement *titleElement = doc.FirstChildElement("doc")->FirstChildElement("title");
    const char *title1 = titleElement->GetText();
    if(title1 != nullptr){
        string title(title1);
        title2 = title;
    }
    XMLElement *desElement = doc.FirstChildElement("doc")->FirstChildElement("description");
    const char *des1 = desElement->GetText();
    if(des1 != nullptr){
        string description(des1);
        description2 = description;
    }
    result = title2 + description2;
    return result;
    
}

void InvertIndex::stopnew(const string &filepath){
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
            dealword(word);
        }
        ++_stop[word];
    }
    ifs.close();
}

void InvertIndex::start(const string &newripepage,const string &newoffset,const string&enstop,const string& zhstop,const string& invertPath)
{
    stopnew(enstop);
    stopnew(zhstop); // 停用词引入

    ifstream offset(newoffset);
    if (!offset)
    {
        cerr << "ifs error stop\n";
    }

    ifstream page(newripepage);
    if (!page)
    {
        cerr << "ifs error stop\n";
    }

    string line;
    int index;
    size_t start, size;

    

    // 每读入一次下标对应，就代表读入一个网页
    int count = 1;
    while (getline(offset, line))
    {
        string content;
        istringstream iss(line);
        iss >> index >> start >> size;
        page.seekg(start, ios::beg);
        content.resize(size);
        char * arr = new char[size+1]{0}; 
        page.read(arr, size);
        content = arr;
        delete[] arr;
        cout <<"read \n";

        string result = this->read(content);

        cout <<"read end\n";
        vector<string> words;
        cout <<"world\n";
        words = _currtor->cut(result); // 中英文已经分词
        cout << count++ <<"\n";
        for (auto &word : words)
        {
            if (_stop[word] < 1) // 不是停用词
            {
                _countPage[word].insert(index);   // 该文章中出现过这个词汇
                ++_count[make_pair(word, index)]; // 该文章中总共出现过多少次这个单词
                _ptow[index].insert(word);//这篇文章有多少单词
            }
        }
        _num++;
        cout <<"hello\n";
    }
    offset.close();
    page.close();
    countPri();
    store(invertPath);
}

void InvertIndex::countPri()
{
    // 计算tfboys
    for (auto res : _countPage)
    {
        double df = res.second.size();
        double tf;
        double idf;
        double w;
        for (auto it = (res.second).begin(); it != (res.second).end(); ++it)
        { // 将单词对应的所有文章取出
            tf = _count[make_pair(res.first, *it)];
            idf = log2(_num / (df + 1));
            w = tf * idf;
            _wn[res.first] = w;
            // _invert[res.first].insert(make_pair(*it, w)); // 倒排索引库
        }
    }
    for(auto res:_ptow){
        double w1;
        double sum = 0;
        for(auto it = (res.second).begin() ; it != (res.second).end();++it){//遍历文章中所有单词取权重
            sum += (_wn[*it])*(_wn[*it]);
        }
        w1 = std::sqrt(sum);
        for(auto it = (res.second).begin() ; it != (res.second).end();++it){//遍历文章中所有单词取权重
            w1 = (_wn[*it]) / std::sqrt(sum);
            _invert[*it].insert(make_pair(res.first, w1));
        }
    }
}

void InvertIndex::store(const string& invertPath){
    ofstream ofs(invertPath);
    if (!ofs)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
    for(auto res:_invert){
        for(auto it = (res.second).begin(); it != (res.second).end();++it){
            ofs << res.first << "\t" << (*it).first << "\t" << (*it).second <<"\n";
        }
    }
}
