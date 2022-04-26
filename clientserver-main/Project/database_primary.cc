#include "database.h"
#include "article.h"

#include <vector>
#include <string>
#include <stdexcept>

using std::vector;
using std::string;
using std::pair;

// är det okej med default?? idk?
database::database() = default;

database::~database() = default;

vector<string> database::list_groups() const{
	vector<string> names;
	for(auto p : groups) {
		names.push_back(p.first);
	}
	return names;
} 

bool database::delete_group(string news_group) {
	auto idx = std::find_if(groups.begin(), groups.end(), 
			[&] (const pair<string, vector<article> > p) {return p.first == news_group;});
	if(idx == groups.end()) {
		return false;
	}
	groups.erase(idx);
	return true;
}

bool database::create_group(string news_group){
    // ska kolla om det redan finns en sådan grupp
    auto it = std::find_if( groups.begin(), groups.end(),
        [&](const pair<string, vector<article> >& element){ return element.first == news_group;} );
    if(it == groups.end()){
        vector<article> articles;
        std::pair<string, vector<article> > group = make_pair(news_group, articles);
        groups.push_back(group);
        return true;
    }else{
        return false;
    }
}

vector<string> database::list_articles(string news_group) const{
    vector<string> article_names;
    auto it = std::find_if( groups.begin(), groups.end(),
        [&](const pair<string, vector<article> >& element){ return element.first == news_group;} );

    if(it == groups.end()){
        throw std::invalid_argument("The requested news group could not be found");
    }else{
        for(auto e : (*it).second){
            article_names.push_back(e.getTitle());
        }
    }
    return article_names;
}

article database::read(string news_group, string article_name) const{
    auto it = std::find_if( groups.begin(), groups.end(),
        [&](const pair<string, vector<article> >& element){ return element.first == news_group;} );

    if(it == groups.end()){
        throw std::invalid_argument("The requested news group cold not be found");
    }else{
        vector<article> articles = (*it).second;
        for(auto i : articles){
            if(i.getTitle() == article_name){
                return i;
            }
        }
        throw std::invalid_argument("The requested article could not be found in the news group");
    }
}

bool database::write(string news_group, article art){
        auto it = std::find_if(groups.begin(), groups.end(),
        [&](const pair<string, vector<article> >& element){ return element.first == news_group;} );

        if(it == groups.end()){
            throw std::invalid_argument("The requested news group could not be found");
        }else{
            (*it).second.push_back(art);
            return true;
        }
}

bool database::delete_article(string news_group, string article_name){
    auto it = std::find_if(groups.begin(), groups.end(),
        [&](const pair<string, vector<article> >& element){ return element.first == news_group;} );
    
    if(it == groups.end()){
        throw std::invalid_argument("The requested news group could not be found");
    }else{
        auto ele = std::find_if( (*it).second.begin(), (*it).second.end(),
            [&](const article& element){ return element.getTitle() == article_name;} );
        if(ele != (*it).second.end()){
            (*it).second.erase(ele);
            return true;
        }
        throw std::invalid_argument("The requested article could not be found in the news group");
    }
}