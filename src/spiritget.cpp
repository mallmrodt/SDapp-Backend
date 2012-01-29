#include <iostream>
#include <string>
#include <curl/curl.h>
#include "include/spiritget.h"

using namespace std;

string linkgen(string sclass)
{
    string url("http://spirit.fh-schmalkalden.de/rest/1.0/schedule?classname=");
    url.append(sclass);

    return url;
}

CURLcode cshedule(string sclass)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    string url(linkgen(sclass));

    rename("schedule.json","_schedule.json");

    fp = fopen("schedule.json","w");

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
    }
    remove("_schedule.json");

    return res;
}
