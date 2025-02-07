#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace tinyxml2;
using namespace std;
int main(){
//     string content = "<doc>"
// 	                "<docid>2</docid>"
// 	"<title>天问一号“奔火” “太空刹车”成功</title>"
// 	"<link>http://scitech.people.com.cn/n1/2021/0212/c1007-32028847.html</link>"
// 	"<description>　　2月10日19时52分，中国首次火星探测任务“天问一号”探测器实施捕获制动，顺利进入近火点高度约400千米、周期约10个地球日、倾角约10°的大椭圆环火轨道，成为我国第一颗人造火星卫星，实现“绕、着、巡”目标的第一步，环绕火星获得成功！　　受访专家表示，首次火星探测任务飞行过程包括发射、地火转移、火星捕获、火星停泊、离轨着陆和科学探测等6个阶段。“奔火”之路上，“天问一号”完成了4次轨道中途修正和1次深空机动。　　据介绍，为了确保“天问一号”能够顺利“奔火”，科研人员为它设计了最优的飞行轨道，制定了详细、全面的控制策略，助力“天问一号”在导航中精准飞行。　　飞抵火星只是第一步，从地火转移轨道进入到环火轨道，需要通过近火制动来实现火星捕获，完成日心轨道和火心轨道的顺利衔接。这一动作也被形象地称为“踩刹车”。　　专家介绍，火星制动捕获是指探测器在抵近火星时，通过主发动机长时间点火，让在行星际空间高速飞行的探测器大速度增量减速，从而能够被火星引力场捕获，进入环火轨道。作为火星探测任务中技术风险最高、技术难度最大的环节之一，制动捕获的机会是唯一的，关系着整个工程任务的成败。　　在完成第四次中途修正后，“天问一号”就进入到了合适的窗口期，它必须精准地踩下刹车，一步到位迈入预定环火轨道。这一步非常关键，只有点火时机和时长都分秒不差，才能形成理想的目标捕获轨道。　　中国航天科技集团八院“天问一号”副总指挥张玉花表示，本次制动捕获的成功，离不开各系统的大力协同、密切配合，不仅考验了地面测控系统深空高精度测定轨能力，还进一步验证了火星环绕器的各项高可靠设计。　　对于大家关心的着陆时间，国家航天局专家表示，“天问一号”计划于今年5—6月择机实施火星着陆，开展巡视探测。着陆巡视器将在火星乌托邦平原南部预定区域实施软着陆。　　中国首次火星探测任务总设计师张荣桥表示，“通过这次任务的实施，在火星使命牵引下的新一轮创新，对实现航天技术领域新跨越、推动我国由航天大国走向航天强国意义重大。” 　　《 人民日报 》（ 2021年02月12日 04 版）</description>"
// "</doc>";


//2367	10658542	28685

    //string newripepage = "/home/dzn/Linux/linux57/webdisk/data/newripepage.dat";
    //ifstream ifs(newripepage);
    ifstream ifs("/home/dzn/Linux/linux57/webdisk/data/test.dat");
    string content;
    content.resize(28685);
    // ifs.seekg(10658542,ios::beg);
    ifs.read(content.data(),28685);
    cout << content<<"\n";
    XMLDocument doc;
    string result;
    static const char *xml = content.c_str();
    doc.Parse(xml);
    XMLElement *titleElement = doc.FirstChildElement("doc")->FirstChildElement("title");
    if(titleElement == nullptr){
        cout << "null 1\n";
    }
    const char *title1 = titleElement->GetText();
    if(titleElement == nullptr){
        cout << "null 2\n";
    }
    
    string title(title1);
    XMLElement *desElement = doc.FirstChildElement("doc")->FirstChildElement("description");
    if(desElement == nullptr){
        cout << "null 3\n";
    }
    const char *des1 = desElement->GetText();
     if(des1 == nullptr){
        cout << "null 4\n";
    }
    string description(des1);
    result = title + description;
    cout <<result<<"\n";
    return 0;
}