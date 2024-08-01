#include "DictProducer.h"
#include "SplitToolCppJieba.h"
int main()
{
    string ch_filepath = "/home/dzn/Linux/linux57/webdisk/split/yuliao/ch";
    string en_filepath = "/home/dzn/Linux/linux57/webdisk/split/yuliao/en";
    string ch_art_filepath = "/home/dzn/Linux/linux57/webdisk/split/yuliao/charticle";
    string ch_stop = "/home/dzn/Linux/linux57/webdisk/split/yuliao/stop";
    string en_stop = "/home/dzn/Linux/linux57/webdisk/split/yuliao/enstop";
    string newripepage = "/home/dzn/Linux/linux57/webdisk/data/newripepage.dat";
    string newoffset = "/home/dzn/Linux/linux57/webdisk/data/newoffset.dat";
    string invertPath = "/home/dzn/Linux/linux57/webdisk/data/invertIndex.dat";
    DictProducer dic(en_filepath);
    dic.buildEnDict();
    // SplitToolCppJieba tool;
    // // DictProducer diczh(ch_filepath,&tool);
    // // diczh.buildCnDict();
    // InvertIndex index(
    //     &tool);
    // index.start(newripepage,
    //             newoffset,
    //             en_stop,
    //             ch_stop,
    //             invertPath);

    return 0;
}
