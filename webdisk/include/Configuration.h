/**
 * Project SearchEngine
 */


#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H


#include <string>
#include <memory>

using namespace std;

class Configuration 
:public std::enable_shared_from_this<Configuration>
{
public:
    using ConfigurationPtr = shared_ptr<Configuration>;
    Configuration();
    string getEnDict();
    string getEnIndexDict();
    string getCnDict();
    string getCnIndexDict();
    string getNewipepage();
    string getNewOffset();
    string getInvertPath();
private:
    string _endict;
    string _enIndex;
    string _chdict;
    string _chIndex;
    string _newripepage;
    string _newoffset;
    string _invertPath;
};

#endif //_CONFIGURATION_H