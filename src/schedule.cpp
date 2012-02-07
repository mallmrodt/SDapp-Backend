#include <json/reader.h>
#include <json/value.h>
#include <vector>
#include <cstdio>
#include <ctime>
#include "include/schedule.h"
#include "include/time_extend.h"

using namespace std;

int weekToInt(string week)
{
    if(week.compare("w")==0) return 2;
    if(week.compare("g")==0) return 0;
    if(week.compare("u")==0) return 1;

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

    return 0;
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

    return -1;
}

int groupToInt(string group)
{
    int rtn;
    char c=0;

    for(unsigned int i=0;i<group.size();i++)
    {
        c = group[i];
        if((c>47)&&(c<58)) break;
    }

    if(c==48) rtn = 0;
    if(c==49) rtn = 1;
    if(c==50) rtn = 2;

    return rtn;
}

vector<string> groupHash()
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

    reader.parse(sched,root,false);

    vector<string> rtn, *prtn=&rtn;

    for(unsigned int i=0;i<root.size();i++)
    {
        int group = groupToInt(root[i].get("group","Missigno").asString());

        if(group!=0)
        {
            if((*prtn).size()==0)
            {
                cout << root[i].get("titleShort","Missigno").asString() << endl;
                (*prtn).push_back(root[i].get("titleShort","Missigno").asString());
            }
            else
            {
                bool exists=false;
                for(unsigned int j=0;j<(*prtn).size();j++)
                {
                    if(root[i].get("titleShort","Missigno").asString() == (*prtn)[j]) exists = true;
                }

                if(!exists)
                {
                    cout << root[i].get("titleShort","Missigno").asString() << endl;
                    (*prtn).push_back(root[i].get("titleShort","Missigno").asString());
                }
            }
        }
    }

    return rtn;
}

schedule::schedule(map<string,int> groups)
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

    reader.parse(sched,root,false);

    for(unsigned int i=0;i<root.size();i++)
    {
        int day, week, time, group;
        week = weekToInt(root[i].get("appointment","").get("week","").asString());
        day = dayToInt(root[i].get("appointment","").get("day","").asString());
        time = timeToInt(root[i].get("appointment","").get("time","").asString());
        group = groupToInt(root[i].get("group","").asString());

        int j=0,h=0;

        if(week==0) {j=0; h = 1;}
        if(week==1) {j=1; h = 2;}
        if(week==2) {j=0; h = 2;}

        map<string,int>::iterator it = groups.find(root[i].get("titleShort","").asString());

        if( ( it!= groups.end() ) &&
            ( group!=it->second ) &&
            ( root[i].get("eventType","notFound").asString()=="Uebung")
            ) continue;

        for(;j<h;j++)
        {
            timetable[j][day][time].eventType = root[i].get("eventType","").asString();
            timetable[j][day][time].eventName = root[i].get("titleShort","").asString();
            timetable[j][day][time].docentName = root[i].get("member","")[0u].get("name","").asString();
            timetable[j][day][time].className = root[i].get("className","").asString();
            timetable[j][day][time].day = root[i].get("appointment","").get("day","").asString();
            timetable[j][day][time].building = root[i].get("appointment","").get("location","").get("building","").asString();
            timetable[j][day][time].room = root[i].get("appointment","").get("location","").get("room","").asString();
            timetable[j][day][time].time = root[i].get("appointment","").get("time","").asString();
            timetable[j][day][time].week = root[i].get("appointment","").get("week","").asString();
            timetable[j][day][time].group = root[i].get("group","").asString();
        }
    }
}

int lTimeToInt(int hour, int min)
{
    if((hour==8)&&(min>=15)) return 0;
    if((hour==9)&&(min<=45)) return 0;

    if(hour==10) return 1;
    if((hour==11)&&(min<=30)) return 1;

    if((hour==11)&&(min>=45)) return 2;
    if(hour==13) return 2;
    if((hour==13)&&(min<=15)) return 2;

    if((hour==14)&&(min>=15)) return 3;
    if((hour==15)&&(min<=45)) return 3;

    if(hour==16) return 4;
    if((hour==17)&&(min<=30)) return 4;

    if((hour==17)&&(min>=45)) return 5;
    if(hour==18) return 5;
    if((hour==19)&&(min<=15)) return 5;

    if((hour==19)&&(min>=30)) return 6;
    if(hour==20) return 6;
    if((hour==21)&&(min==00)) return 6;

    return -1;
}

event * schedule::getActual()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int week, day, ltime;

    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday; day--;
    if(day<0) day += 7;

    ltime = lTimeToInt((*timeinfo).tm_hour,(*timeinfo).tm_min);

    if((ltime==-1)||(timetable[week][day][ltime].eventName.size()==0))
    {
        return NULL;
    }

    event *rtn = new event;
    *rtn = timetable[week][day][ltime];

    return rtn;
}

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

    week += 1;
    if(week<1) week -= 2;
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

    week = weekofyear(*timeinfo) %2;
    day = (*timeinfo).tm_wday; day--;
    if(day>6) day += 7;

    ltime = lTimeToInt((*timeinfo).tm_hour,(*timeinfo).tm_min);

    for(int i=(ltime+1);i<7;i++)
    {
        if(timetable[week][day][i].eventName.size()!=0)
        {
            struct tm timetemp = toTime(i);

            return ((timetemp.tm_hour-(*timeinfo).tm_hour)*60*60+(timetemp.tm_min-(*timeinfo).tm_min)*60-(*timeinfo).tm_sec);
        }
    }

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

    week += 1;
    if(week<1) week -= 2;
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

void schedule::getTimetable(event tcopy[2][7][7])
{
    for(int i=0;i<2;i++) for(int j=0;j<7;j++) for(int k=0;k<7;k++)
    {
        tcopy[i][j][k] = timetable[i][j][k];
    }
}

void schedule::getActualDay(event tcopy[7])
{
    if(getActual()!=NULL)
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

    if(getNext()!=NULL)
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

        for(int i=(time+1);i<7;i++)
        {
            if(timetable[week][day][i].eventName.size()!=0)
            {
                for(int l=0;l<7;l++)
                {
                    tcopy[l] = timetable[week][day][l];
                }
                return;
            }
        }

        for(int i=(day+1);i<7;i++)
        {
            for(int j=0;j<7;j++)
            {
                if(timetable[week][i][j].eventName.size()!=0)
                {
                    for(int l=0;l<7;l++)
                    {
                        tcopy[l] = timetable[week][i][l];
                    }
                    return;
                }
            }
        }

        week += 1;
        if(week<1) week -= 2;
        for(int i=(day+1);i<7;i++)
        {
            for(int j=0;j<7;j++)
            {
                if(timetable[week][j][i].eventName.size()!=0)
                {
                    for(int l=0;l<7;l++)
                    {
                        tcopy[l] = timetable[week][j][l];
                    }
                    return;
                }
            }
        }
    }
}
