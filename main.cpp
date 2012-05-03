#include <iostream>
#include <map>

#include "spirit/spiritget.h"
#include "spirit/schedule.h"
#include "spirit/news.h"
#include "spirit/exception.h"

using namespace std;

void runSched()
{
    schedule sched;                                          //class initialisation

    //sched.download("bai2");

    sched.initJson();

    {
        vector<string> tmp = sched.groupHash();                             //returns the events divided in groups

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

    sched.initGrp(hashtable);                                   //Initialisation of 3D array, excluding events of different group(s) according to hashtable

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
}

void runNews()
{
    news fhNews;

    //fhNews.cdownload();

    fhNews.init();

    vector<article> bai2 = fhNews.get("BAI2");

    for(unsigned int i=0;i<bai2.size();i++)
    {
        cout << bai2[i].subject << endl;
    }
}

int main()
{
    try
    {
        runSched();
    }
    catch(spError exc)
    {
        cout << "ScheduleException: " << exc.errorCode << endl;
    }
    catch(...)
    {
        cout << "UNKNOWN_EXCEPTION_SCHEDULE";
    }

    cout << endl << endl;

    try
    {
        runNews();
    }
    catch(spError exc)
    {
        cout << "NewsException: " << exc.errorCode << endl;
    }
    catch(...)
    {
        cout << "UNKNOWN_EXCEPTION_NEWS";
    }

    return 0;
}
