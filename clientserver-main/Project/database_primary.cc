#include "database.h"
#include "article.h"
#include "newsgroup.h"

// own exceptions
#include "badngnumber.h"
#include "badartnumber.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::endl;

database::database() = default;

database::~database() = default;

vector<pair<int, string> > database::list_groups() const{
	vector<pair<int, string> > names;
	for(auto p : groups) {
		names.push_back(std::make_pair(p.get_id_nbr(), p.get_title()));
	}
	return names;
} 

bool database::delete_group(int ng_id_nbr) {
	auto idx = std::find_if(groups.begin(), groups.end(), 
			[ng_id_nbr] (const Newsgroup ng) {return ng.get_id_nbr() == ng_id_nbr;});
	if(idx == groups.end()) {
		return false;
	}
	groups.erase(idx);
	return true;
}

bool database::create_group(string ng_name){
    // ska kolla om det redan finns en sådan grupp
    auto it = std::find_if( groups.begin(), groups.end(),
        [ng_name](const Newsgroup ng){ return ng.get_title() == ng_name;} );
    if(it == groups.end()){
        Newsgroup group(ctr++, ng_name); // init newsgroup
        groups.push_back(group);
        return true;
    }else{
        return false;
    }
}

vector<pair<int, string> > database::list_articles(int ng_id_nbr) const{
    vector<pair<int, string> > articles;
    auto it = std::find_if( groups.begin(), groups.end(),
        [ng_id_nbr](const Newsgroup ng){ return ng.get_id_nbr() == ng_id_nbr;} );

    if(it == groups.end()){
        throw BadNGException();
    }else{
        for(auto e : (*it).get_articles()){
            articles.push_back(std::make_pair(e.getIdNbr(), e.getTitle()));
        }
    }
    return articles;
}

bool database::read(int ng_id_nbr, int art_id_nbr, string& title, string& author, string& text) const{
    auto ng_it = std::find_if( groups.begin(), groups.end(),
        [ng_id_nbr](const Newsgroup ng){ return ng.get_id_nbr() == ng_id_nbr;} );

    if(ng_it == groups.end()){
        throw BadNGException();
    }else{
        vector<Article> articles = ng_it->get_articles();
        auto art_it = std::find_if( articles.begin(), articles.end(),
        [art_id_nbr](const Article art){ return art.getIdNbr() == art_id_nbr;} );
        
        if(art_it == articles.end()){
            throw BadARTException();
        } else {
            title = art_it->getTitle();
            author = art_it->getAuthor();
            text = art_it->getText();
        }
    }
    return true;
}

bool database::write(int ng_id_nbr, string title, string author, string text){
        auto it = std::find_if(groups.begin(), groups.end(),
        [ng_id_nbr](const Newsgroup ng){ return ng.get_id_nbr() == ng_id_nbr;} );

        if(it == groups.end()){
            throw BadNGException();
        }else{
            Article art(title, author, text, ctr++);
            it->add_article(art);
        }
        return true;
}

bool database::delete_article(int ng_id_nbr, int art_id_nbr){
    auto ng_it = std::find_if( groups.begin(), groups.end(),
        [ng_id_nbr](const Newsgroup ng){ return ng.get_id_nbr() == ng_id_nbr;} );

    if(ng_it == groups.end()){
        throw BadNGException();
    }else{
        int success = ng_it->remove_article(art_id_nbr);
        if (success){
            return true;
        } else {
            throw BadARTException();
        }
    }
    return false;
}
