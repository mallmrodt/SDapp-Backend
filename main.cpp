#include <iostream>
#include <map>
#include "spiritget.h"
#include "schedule.h"
#include "news.h"

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

void blub()
{
    //cshedule("bai1");

    map<string,int> hashtable;

    hashtable[readfile()]=1;
    hashtable["Gr InfV"]=1;

    schedule timetable(hashtable);

    /*
    event ptimetable[2][7][7];
    timetable.getTimetable(ptimetable);

    for(int i=0;i<2;i++)
    {
        for(int j=0;j<7;j++)
        {
            for(int k=0;k<7;k++)
            {
                cout << i << j << k << " "<<ptimetable[i][j][k].eventName << endl;
            }
        }
    }
    */

    event tp[7];
    timetable.getActualDay(tp);
    for(int i=0;i<7;i++)
    {
        cout << tp[i].eventName << endl;
    }

    /*
    vector<string> blub = grouphash();

    for(unsigned int i;i<blub.size();i++)
    {
        cout << blub[i] << endl;
    }
    */

    /*
    event *blub = timetable.getActual();

    if(blub!=NULL)
    {
        cout << (*blub).eventName << endl;
        cout << (*blub).day << endl;
        cout << (*blub).time << endl << endl;
    }

    blub = timetable.getNnext();

    if(blub!=NULL)
    {
        cout << (*blub).eventName << endl;
        cout << (*blub).day << endl;
        cout << (*blub).time << endl;
    }

    cout << timetable.timeDifference() << endl;
    */

}

int main()
{
    if(cshedule("bai1")!=0) return 1;

    map<string,int> hashtable;

    hashtable[readfile()]=1;
    hashtable["Gr InfV"]=1;

    schedule timetable(hashtable);

    event ptimetable[2][7][7];
    timetable.getTimetable(ptimetable);

    for(int i=0;i<2;i++)
    {
        for(int j=0;j<7;j++)
        {
            for(int k=0;k<7;k++)
            {
                cout << i << j << k << " "<<ptimetable[i][j][k].eventName << endl;
            }
        }
    }

    /*event tp[7];
    timetable.getActualDay(tp);
    for(int i=0;i<7;i++)
    {
        cout << tp[i].eventName << endl;
    }*/

    /*
    vector<string> blub = grouphash();

    for(unsigned int i;i<blub.size();i++)
    {
        cout << blub[i] << endl;
    }
    */

    event *blub = timetable.getActual();

    if(blub!=NULL)
    {
        cout << (*blub).eventName << endl;
        cout << (*blub).day << endl;
        cout << (*blub).time << endl << endl;
    }

    blub = timetable.getNext();

    if(blub!=NULL)
    {
        cout << (*blub).eventName << endl;
        cout << (*blub).day << endl;
        cout << (*blub).time << endl;
    }

    cout << timetable.timeDifference() << endl;


    /*
    news journal;

    vector<article> filtered = journal.get("alle");

    for(unsigned int i=0;i<filtered.size();i++)
    {
        cout << filtered[i].subject << endl;
    }
    */

    /*
    vector<article> blub = journal.get("");

    for(unsigned int i=0;i<blub.size();i++)
    {
        cout << blub[i].nr << endl;
    }
    */

    return 0;
}

/*cout << root[0u].get("member","___")[0u].get("name","...").asString() << endl;
cout << root.size() << endl;*/

//cout << root[0u].get("name","").asString() << endl;
