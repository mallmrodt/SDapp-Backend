#include <iostream>
#include <map>
#include "include/spiritget.h"
#include "include/schedule.h"

using namespace std;

string readfile()
{
    string rtn;
    FILE *fp = fopen("blub","r");
    char c = fgetc(fp);

    while(c!=-1)
    {
        rtn += c;
        c = fgetc(fp);
    }

    return rtn;
}

int main()
{
    cshedule("bai1");

    map<string,int> hashtable;

    hashtable[readfile()]=1;
    hashtable["Gr InfV"]=1;

    schedule timetable(hashtable);

    for(int i=0;i<2;i++)
    {
        for(int j=0;j<7;j++)
        {
            for(int k=0;k<7;k++)
            {
                cout << timetable.timetable[i][j][k].eventName << endl;
            }
        }
    }

    vector<string> blub = grouphash();

    for(unsigned int i;i<blub.size();i++)
    {
        cout << blub[i] << endl;
    }

    return 0;
}

/*cout << root[0u].get("member","___")[0u].get("name","...").asString() << endl;
cout << root.size() << endl;*/

//cout << root[0u].get("name","").asString() << endl;
