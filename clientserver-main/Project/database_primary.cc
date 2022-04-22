#include "database.h"
#include "article.h"

#include <vector>
#include <string>

using std::vector;
using std::string;
using std::pair;

bool database::create_group(string news_group){
    // ska kolla om det redan finns en sådan grupp
    auto it = std::find_if( groups.begin(), groups.end(),
        [news_group&](const pair<string, vector<article> >& element){ return element.first == news_group;} );
    if(it == groups.end()){
        vector<article> articles();
        std::pair<string, vector<article> > group = make_pair(news_group, articles);
        groups.push_back(group);
        return true;
    }else{
        return false;
    }
}

vector<string> database::list_articles(string news_group) const{
    vector<string> group_names();
    for(auto e : groups){
        group_names.push_back(e.first);
    }
    return group_names;
}

article database::read(string news_group, string article_name) const{
    
}