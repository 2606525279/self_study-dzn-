/**
 * Project SearchEngine
 */

#include "../include/Configuration.h"

/**
 * Configuration implementation
 */

Configuration::Configuration()
{
    _endict = "/home/dzn/Linux/linux57/webdisk/data/endict.dat";
    _enIndex = "/home/dzn/Linux/linux57/webdisk/data/endictIndex.dat";
    _chdict = "/home/dzn/Linux/linux57/webdisk/data/chdict.dat";
    _chIndex = "/home/dzn/Linux/linux57/webdisk/data/chdictIndex.dat";
    _newripepage = "/home/dzn/Linux/linux57/webdisk/data/newripepage.dat";
    _newoffset = "/home/dzn/Linux/linux57/webdisk/data/newoffset.dat";
    _invertPath = "/home/dzn/Linux/linux57/webdisk/data/invertIndex.dat";
}

string Configuration::getEnDict() {
    return _endict;
}
string Configuration::getCnDict() {
    return _chdict;
}
string Configuration::getNewipepage() {
    return _newripepage;
}
string Configuration::getNewOffset() {
    return _newoffset;
}
string Configuration::getInvertPath() {
    return _invertPath;
}
string Configuration::getEnIndexDict(){
    return _enIndex;
}
string Configuration::getCnIndexDict(){
    return _chIndex;
}