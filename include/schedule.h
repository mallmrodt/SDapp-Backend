#ifndef SCHEDULE_H
#define SCHEDULE_H

#include<vector>

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

vector<string> grouphash();

class schedule
{
    public:
        schedule(map<string,int> groups);
        event * getactual();
        event * getnext();
        double timedifference();;

    private:
        unsigned int size;
        event timetable[2][7][7];
};

#endif // SCHEDULE_H
