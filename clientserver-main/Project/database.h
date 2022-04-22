#ifndef DATABASE_H
#define DATABASE_H

#include "article.h"
#include <string>
#include <vector> 
using std::string;
using std::vector;

class database{
    public:
        vector<string> list_groups() const;
        bool create_group(string news_group);
        bool delete_group(string news_group);
        vector<string> list_articles(string news_group) const;
        article read(string news_group, string article_name) const;
        bool write(string news_group, article art);
        bool delete_article(string news_group, string art);
    private:
        vector<std::pair<string, vector<article> > > groups;
};



#endif