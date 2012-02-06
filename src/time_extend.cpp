#include <time.h>

int weekofyear(struct tm localtime)
{
    int wDayOnFirst;
    int wKYear;

    wDayOnFirst = localtime.tm_wday - (localtime.tm_yday%7);
    if(wDayOnFirst<0) wDayOnFirst += 7;

    wKYear = 1 + localtime.tm_yday / 7;
    if(wDayOnFirst>3) wKYear++;
    if(localtime.tm_wday==0) wKYear--;

    if(wKYear==0)
    {
        struct tm last = localtime;

        last.tm_year--;
        last.tm_wday--;

        if(last.tm_wday<0) last.tm_wday += 7;
        if((last.tm_year%4)==0) last.tm_yday = 365;
        else last.tm_yday = 364;

        wKYear = weekofyear(last);
    }

    return wKYear;
}
