#ifndef SPIRITGET_H
#define SPIRITGET_H
#include <curl/curl.h>
#include <string>

CURLcode cshedule(std::string sclass);

CURLcode cnews();

#endif // SPIRITGET_H
