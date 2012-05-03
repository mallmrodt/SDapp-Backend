#ifndef SCHEDULE_H
#define SCHEDULE_H

#include<vector>
#include<map>

#include "spirit/spiritget.h"

using namespace std;

typedef struct event
{
    string eventType;
    string eventName;
    string docentName;
    string className;
    string day;
    string building;
    string room;
    string time;
    string week;
    string group;
}event;

class schedule
{
    public:
        void initJson();
        void download(string sclass) {cschedule(sclass);};
        vector<string> groupHash();
        void initGrp(map<string,int> groups);
        event * getActual();
        event * getNext();
        double timeDifference();
        void getTimetable(event tcopy[2][7][7]);
        void getActualDay(event tcopy[7]);

    private:
        vector<event> evList;
        event timetable[2][7][7];
};

#endif // SCHEDULE_H
