#ifndef NEWS_H
#define NEWS_H

#include <vector>

#include "spirit/spiritget.h"

using namespace std;

typedef struct article
{
    string semester;
    string name;
    string subject;
    string writer;
    string lifecycle;
    string nr;
    string id;
    string date;
    string news;
}article;

class news
{
    public:
        void init();
        void cdownload() {cnews();};
        vector<article> get(string filter);

    private:
        vector<article> journal;
        void sort();
};

#endif // NEWS_H
