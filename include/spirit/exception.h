#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

class spError
{
    public:
        enum Code
        {
            NO_ERROR,           //0; unused, only for formatting
            TIME_PARSE_ERROR,   //1
            DAY_PARSE_ERROR,    //2
            WEEK_PARSE_ERROR,   //3
            GROUP_PARSE_ERROR,  //4
            JSON_ERROR,         //5
            CURL_ERROR,         //6
            SAVE_FAIL_ERROR,    //7
            HASH_MAP_ERROR,     //8
            NO_EVENT_ERROR,     //9
            NO_NEWS_ERROR,      //10
            FALSE_NAMING_ERROR  //11
        };

        Code errorCode;
        int curlError;

        spError(Code eC)
        {
            errorCode = eC;
            curlError = -1;
        }

        spError(Code eC, int cE)
        {
            errorCode = eC;
            curlError = cE;
        }
};

#endif // EXCEPTION_H_INCLUDED
