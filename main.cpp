#include <iostream>
#include <map>
#include "spiritget.h"
#include "schedule.h"
#include "news.h"

using namespace std;

int main()
{
    if(cshedule("bai2")!=0) return 1;                        //curl instruction for downloading schedule of 'bai2'

    schedule sched;                                          //class initialisation

    {
        vector<string> tmp;
        tmp = sched.groupHash();                             //returns the events divided in groups

        for(unsigned int i=0;i<tmp.size();i++) cout << tmp[i] << endl;
        cout << endl;
    }

    map<string,int> hashtable;                              //Hashtable for initalisation
    {
        hashtable["OOP"]=1;
        hashtable["MPT"]=1;
        hashtable["Afs"]=1;
        hashtable["SWE"]=1;
        hashtable["AgK Ma"]=1;
    }

    sched.init(hashtable);                                   //Initialisation of 3D array, excluding events of different group(s) according to hashtable

    {
        event * tmp;

        tmp = sched.getActual();
        if(tmp!=NULL) cout << tmp->eventName << endl;

        tmp = sched.getNext();
        if(tmp!=NULL) cout << tmp->eventName;

        int h, min, sec, diff = sched.timeDifference();     //timeDifference() returns time till next event in seconds
        h = diff/3600; diff -= h*3600;
        min = diff / 60; diff -= min*60;
        sec = diff;

        cout << "\t" << h << "h: "  << min << "min: " << sec << "sec" << endl;
    }

    {
        event evToday[7];
        sched.getActualDay(evToday);          //getActualDay() fills 1D-array of events on the actual day (according Initialisation)

        event evComplete[2][7][7];
        sched.getTimetable(evComplete);   //getTimetable() fills 3D-array of all events (accodring Initalisation)
    }

    return 0;
}

/*cout << root[0u].get("member","___")[0u].get("name","...").asString() << endl;
cout << root.size() << endl;*/

//cout << root[0u].get("name","").asString() << endl;
