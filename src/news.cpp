#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>

#include <json/reader.h>
#include <json/value.h>

#include "spirit/news.h"
#include "spirit/exception.h"

using namespace std;

void news::sort()
{
    bool changed=true;

    while(changed)
    {
        changed = false;

        for(int i=0;i<(journal.size()-1);i++)
        {
            if(strcmp(journal[i].nr.c_str(),journal[i+1].nr.c_str())<0)
            {
                article h = journal[i];
                journal[i] = journal[i+1];
                journal[i+1] = h;

                changed = true;
            }
        }
    }
}

void news::init()
{
    Json::Value root;
    Json::Reader reader;
    FILE *fp = fopen("news.json","r");
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
        article newArticle;
        string text;

        newArticle.semester = root[i].get("semester","Missigno").asString();
        newArticle.name = root[i].get("name","Missigno").asString();
        newArticle.subject = root[i].get("subject","none").asString();
        newArticle.writer = root[i].get("writer","Missigno").asString();
        newArticle.lifecycle = root[i].get("lifecycle","Missigno").asString();
        newArticle.nr = root[i].get("nr","Missigno").asString();
        newArticle.id = root[i].get("_id","Missigno").asString();
        newArticle.date = root[i].get("date","Missigno").asString();
        newArticle.news = root[i].get("news","Missigno").asString();

        journal.push_back(newArticle);
    }

    if(journal.size()==0)
    {
        spError exc(spError::NO_NEWS_ERROR);
        throw(exc);
    }

    for(unsigned int i=0;i<journal.size();i++)
    {
        if((journal[i].semester=="Missigno") ||
           (journal[i].name=="Missigno") ||
           (journal[i].subject=="Missigno") ||
           (journal[i].writer=="Missigno") ||
           (journal[i].lifecycle=="Missigno") ||
           (journal[i].nr=="Missigno") ||
           (journal[i].id=="Missigno") ||
           (journal[i].date=="Missigno") ||
           (journal[i].news=="Missigno"))
        {
            spError exc(spError::FALSE_NAMING_ERROR);
            throw(exc);
        }
    }

    sort();
}

vector<article> news::get(string filter)
{
    char c=1,*cp;
    vector<article> rtn;

    for(unsigned int i=0;i<journal.size();i++)
    {

        cp = &journal[i].semester[0];
        vector<string> semesters;

        while(c!='\0')
        {
            string s;

            while(c!=32)
            {
                c = *cp;
                if(((c>47)&&(c<58))||((c>96)&&(c<133))||((c>64)&&(c<91))||(c==95)) s.push_back(c);
                cp++;
            }

            semesters.push_back(s);
            c= *cp;
        }

        for(unsigned int j=0;j<semesters.size();j++)
        {
            if(strcmp(filter.c_str(),"alle")==0)
            {
                rtn.push_back(journal[i]);
                break;
            }
            if((strcmp(filter.c_str(),semesters[j].c_str())==0)||(strcmp("semester",semesters[j].c_str())==0))
            {
                rtn.push_back(journal[i]);
                break;
            }
        }

        c = '0';
    }

    return rtn;
}
