#include <json/reader.h>
#include <json/value.h>
#include <vector>
#include <stdio.h>
#include "include/schedule.h"

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
    if(time.compare("8.15-9.45")==0) return 0;
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

vector<string> grouphash()
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

    vector<string> rtn;

    for(unsigned int i=0;i<root.size();i++)
    {
        int group = groupToInt(root[i].get("group","Missigno").asString());

        if(group!=0)
        {
            if(rtn.size()==0) rtn.push_back(root[i].get("titleShort","Missigno").asString());
            else
            {
                bool exists=false;
                for(unsigned int j=0;j<rtn.size();j++)
                {
                    if(root[i].get("titleShort","Missigno").asString() == rtn.at(j)) exists = true;
                }

                if(!exists) rtn.push_back(root[i].get("titleShort","Missigno").asString());
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
