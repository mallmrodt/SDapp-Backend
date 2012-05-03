#include <vector>
#include <cstdio>
#include <ctime>
#include <stdlib.h>

#include <json/reader.h>
#include <json/value.h>

#include "spirit/schedule.h"
#include "spirit/time_extend.h"
#include "spirit/exception.h"

using namespace std;

int weekToInt(string week)
{
    if(week.compare("w")==0) return 2;
    if(week.compare("g")==0) return 0;
    if(week.compare("u")==0) return 1;

    spError exc(spError::WEEK_PARSE_ERROR);
    throw(exc);

    return -1;
}

int dayToInt(string day)
{
    if(day.compare("Montag")==0) return 0;
    if(day.compare("Dienstag")==0) return 1;
    if(day.compare("Mittwoch")==0) return 2;
    if(day.compare("Donnerstag")==0) return 3;
    if(day.compare("Freitag")==0) return 4;
    if(day.compare("Samstag")==0) return 5;
    if(day.compare("Sonntag")==0) return 6;

    spError exc(spError::DAY_PARSE_ERROR);
    throw(exc);

    return -1;
}

int timeToInt(string time)
{
    if(time.compare("08.15-09.45")==0) return 0;
    if(time.compare("10.00-11.30")==0) return 1;
    if(time.compare("11.45-13.15")==0) return 2;
    if(time.compare("14.15-15.45")==0) return 3;
    if(time.compare("16.00-17.30")==0) return 4;
    if(time.compare("17.45-19.15")==0) return 5;
    if(time.compare("19.30-21.00")==0) return 6;

    spError exc(spError::TIME_PARSE_ERROR);
    throw(exc);

    return -1;
}

int groupToInt(string group)
{
    int rtn=-1;
    char c=0;

    for(unsigned int i=0;i<group.size();i++)
    {
        c = group[i];
        if((c>47)&&(c<58)) break;
    }

    rtn = atoi(&c);

    if((rtn<0)||(rtn>9))
    {
        spError exc(spError::GROUP_PARSE_ERROR);
        throw(exc);
    }

    return rtn;
}

vector<string> schedule::groupHash()
{
    vector<string> rtn;

    for(unsigned int i=0;i<evList.size();i++)
    {
        int group = groupToInt(evList[i].group);

        if(group!=0)
        {
            if(rtn.size()==0)
            {
                //cout << evList[i].eventName << endl;
                rtn.push_back(evList[i].eventName);
            }
            else
            {
                bool exists=false;
                for(unsigned int j=0;j<rtn.size();j++)
                {
                    if(evList[i].eventName == rtn[j]) exists = true;
                }

                if(!exists)
                {
                    //cout << evList[i].eventName << endl;
                    rtn.push_back(evList[i].eventName);
                }
            }
        }
    }

    return rtn;
}

void schedule::initJson()
{
    Json::Value root;
    Json::Reader reader;
    FILE *fp = fopen("schedule.json","r");
    char c=fgetc(fp);
    string sched;

    while(c!=-1)
    {
        sched.append(1,c);
        c = fgetc(fp);
    };

    fclose(fp);

    if(!reader.parse(sched,root,false))
    {
        spError exc(spError::JSON_ERROR);
        throw(exc);
    }

    for(unsigned int i=0;i<root.size();i++)
    {
        event temp;

        temp.eventType = root[i].get("eventType","Missigno").asString();
        temp.eventName = root[i].get("titleShort","Missigno").asString();
        temp.docentName = root[i].get("member","")[0u].get("name","Missigno").asString();
        temp.className = root[i].get("className","Missigno").asString();
        temp.day = root[i].get("appointment","Missigno").get("day","Missigno").asString();
        temp.building = root[i].get("appointment","").get("location","Missigno").get("place","Missigno").get("building","Missigno").asString();
        temp.room = root[i].get("appointment","").get("location","").get("place","").get("room","Missigno").asString();
        temp.time = root[i].get("appointment","").get("time","Missigno").asString();
        temp.week = root[i].get("appointment","").get("week","Missigno").asString();
        temp.group = root[i].get("group","Missigno").asString();

        evList.push_back(temp);
    }

    for(unsigned int i=0;i<evList.size();i++)
    {
        if((evList[i].eventType=="Missigno") ||
           (evList[i].eventName=="Missigno") ||
           (evList[i].docentName=="Missigno") ||
           (evList[i].className=="Missigno") ||
           (evList[i].day=="Missigno") ||
           (evList[i].building=="Missigno") ||
           (evList[i].room=="Missigno") ||
           (evList[i].time=="Missigno") ||
           (evList[i].week=="Missigno") ||
           (evList[i].group=="Missigno"))
        {
            spError exc(spError::FALSE_NAMING_ERROR);
            throw(exc);
        }
    }
}

void schedule::initGrp(map<string,int> groups)
{
    for(unsigned int i=0;i<evList.size();i++)
    {
        int day, week, time, group;
        week = weekToInt(evList[i].week);
        day = dayToInt(evList[i].day);
        time = timeToInt(evList[i].time);
        group = groupToInt(evList[i].group);

        int j=0,h=0;

        if(week==0) {j=0; h = 1;}
        if(week==1) {j=1; h = 2;}
        if(week==2) {j=0; h = 2;}

        map<string,int>::iterator it = groups.find(evList[i].eventName);

        if( ( it!= groups.end() ) &&
            ( group!=it->second ) &&
            ( evList[i].eventType=="Uebung")
            ) continue;

        for(;j<h;j++)
        {
            timetable[j][day][time].eventType = evList[i].eventType;
            timetable[j][day][time].eventName = evList[i].eventName;
            timetable[j][day][time].docentName = evList[i].docentName;
            timetable[j][day][time].className = evList[i].className;
            timetable[j][day][time].day = evList[i].day;
            timetable[j][day][time].building = evList[i].building;
            timetable[j][day][time].room = evList[i].room;
            timetable[j][day][time].time = evList[i].time;
            timetable[j][day][time].week = evList[i].week;
            timetable[j][day][time].group = evList[i].group;
        }
    }
}

//determining current time
int lTimeToInt(int hour, int min)
{
    if(hour<8) return -1;
    if((hour==8)&&(min<15)) return -1;

    if((hour==8)&&(min>=15)) return 0;
    if((hour==9)&&(min<=59)) return 0;

    if(hour==10) return 1;
    if((hour==11)&&(min<44)) return 1;

    if((hour==11)&&(min>=45)) return 2;
    if(hour==12) return 2;
    if((hour==13)&&(min<15)) return 2;

    if((hour==14)&&(min>=15)) return 3;
    if((hour==15)&&(min<=59)) return 3;

    if(hour==16) return 4;
    if((hour==17)&&(min<45)) return 4;

    if((hour==17)&&(min>=45)) return 5;
    if(hour==18) return 5;
    if((hour==19)&&(min<30)) return 5;

    /* last event or later time doesn't matter for program
    if((hour==19)&&(min>=30)) return 6;
    if(hour==20) return 6;
    if((hour==21)&&(min==00)) return 6;
    */

    return 6;
}

//getting event at current time
event * schedule::getActual()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int week, day, ltime;

    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday;day--;
    if(day<0) day += 7;

    ltime = lTimeToInt((*timeinfo).tm_hour,(*timeinfo).tm_min);

    if((ltime==-1)||(timetable[week][day][ltime].eventName.size()==0)) return NULL;

    event *rtn = new event;
    *rtn = timetable[week][day][ltime];

    return rtn;
}

//getting next event that will be held
event * schedule::getNext()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int week, day, time;

    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday; day--;
    if(day>6) day += 7;

    time = lTimeToInt((*timeinfo).tm_hour,(*timeinfo).tm_min);

    event *rtn = NULL;

    //looking for event on same day
    if(time!=6&&day!=6)
    {
        for(int i=(time+1);i<7;i++)
        {
            if(timetable[week][day][i].eventName.size()!=0)
            {
                if(timetable[week][day][i].eventName.size()!=0)
                {
                    event *rtn = new event;
                    *rtn = timetable[week][day][i];
                    return rtn;
                }
            }
        }
    }

    //loking for events on same week
    if(day!=6)
    {
        for(int i=(day+1);i<7;i++)
        {
            for(int j=0;j<7;j++)
            {
                if(timetable[week][i][j].eventName.size()!=0)
                {
                    event *rtn = new event;
                    *rtn = timetable[week][i][j];
                    return rtn;
                }
            }
        }
    }


    //looking for events on next week
    week += 1;
    if(week>1) week -= 2;
    for(int i=0;i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(timetable[week][i][j].eventName.size()!=0)
            {
                event *rtn = new event;
                *rtn = timetable[week][i][j];
                return rtn;
            }
        }
    }

    spError exc(spError::NO_EVENT_ERROR);
    throw(exc);

    return rtn;
}

tm toTime(int ltime)
{
    struct tm timeinfo;

    if(ltime == 0)
    {
        timeinfo.tm_hour = 8;
        timeinfo.tm_min = 15;
    }

    if(ltime == 1)
    {
        timeinfo.tm_hour = 10;
        timeinfo.tm_min = 0;
    }

    if(ltime == 2)
    {
        timeinfo.tm_hour = 11;
        timeinfo.tm_min = 45;
    }

    if(ltime == 3)
    {
        timeinfo.tm_hour = 14;
        timeinfo.tm_min = 15;
    }

    if(ltime == 4)
    {
        timeinfo.tm_hour = 16;
        timeinfo.tm_min = 00;
    }

    if(ltime == 5)
    {
        timeinfo.tm_hour = 17;
        timeinfo.tm_min = 45;
    }

    if(ltime == 6)
    {
        timeinfo.tm_hour = 19;
        timeinfo.tm_min = 30;
    }

    return timeinfo;
}

double schedule::timeDifference()
{
    time_t rawtime;
    struct tm *timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int week, day, ltime;

    //getting current week & day of the week
    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday; day--;
    if(day>6) day -= 7;

    //getting current time; falsely called local time :/
    ltime = lTimeToInt((*timeinfo).tm_hour,(*timeinfo).tm_min);

    //locing for event on the same day
    for(int i=(ltime+1);i<7;i++)
    {
        if(timetable[week][day][i].eventName.size()!=0)
        {
            struct tm timetemp = toTime(i);

            cout << timetemp.tm_hour << (*timeinfo).tm_hour << endl;

            return ((timetemp.tm_hour-(*timeinfo).tm_hour)*60*60+(timetemp.tm_min-(*timeinfo).tm_min)*60-(*timeinfo).tm_sec);
        }
    }

    //looking for events on remaining week
    for(int i=(day+1);i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(timetable[week][i][j].eventName.size()!=0)
            {
                struct tm timetemp = toTime(j);

                return ((i-day)*60*60*24+(timetemp.tm_hour-(*timeinfo).tm_hour)*60*60+(timetemp.tm_min-(*timeinfo).tm_min)*60-(*timeinfo).tm_sec);
            }
        }
    }

    //looking for events on next week
    week += 1;
    if(week>1) week -= 2;
    for(int i=0;i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(timetable[week][i][j].eventName.size()!=0)
            {
                struct tm timetemp = toTime(j);

                return ((60*60*24*7)+(i-day)*60*60*24+(timetemp.tm_hour-(*timeinfo).tm_hour)*60*60+(timetemp.tm_min-(*timeinfo).tm_min)*60-(*timeinfo).tm_sec);
            }
        }
    }

    return -1;
}

//getting complete timetable (minds group)
void schedule::getTimetable(event tcopy[2][7][7])
{
    for(int i=0;i<2;i++) for(int j=0;j<7;j++) for(int k=0;k<7;k++)
    {
        tcopy[i][j][k] = timetable[i][j][k];
    }
}

//getting events on today (minds group)
void schedule::getActualDay(event tcopy[7])
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int week, day;

    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday; day--;
    if(day<0) day += 7;

    for(int i=0;i<7;i++)
    {
        tcopy[i] = timetable[week][day][i];
    }
    return;
}
