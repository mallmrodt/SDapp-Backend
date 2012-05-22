#include <iostream>
#include <string>

#include <curl/curl.h>

#include "spirit/spiritget.h"
#include "spirit/exception.h"

using namespace std;

string linkgen(string sclass)
{
    string url("http://spirit.fh-schmalkalden.de/rest/1.0/schedule?classname=");
    url.append(sclass);

    return url;
}

void cschedule(string sclass)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    string url(linkgen(sclass));

    rename("schedule.json","_schedule.json");

    fp = fopen("schedule.json","w");
    if(fp==NULL)
    {
        spError exc(spError::SAVE_FAIL_ERROR);
        throw(exc);
    }

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    fclose(fp);

    if(res!=0)
    {
        remove("schedule.json");
        rename("_schedule.json","schedule.json");

        spError exc(spError::CURL_ERROR,res);
        throw(exc);
    }
    remove("_schedule.json");
}

void cnews()
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    string url("http://spirit.fh-schmalkalden.de/rest/1.0/news");

    rename("news.json","_news.json");

    fp = fopen("news.json","w");

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    fclose(fp);

    if(res!=0)
    {
        remove("news.json");
        rename("_news.json","news.json");

        spError exc(spError::CURL_ERROR,res);
        throw(exc);
    }
    remove("_news.json");
}
