#include "newsgroup.h"
#include "article.h"

#include <vector>

using std::string;
using std::vector;

Newsgroup::Newsgroup(int ng_id_nbr, string title) : title{title}, id_nbr{ng_id_nbr}, articles{} {}

string Newsgroup::get_title() const {
	return title;
}

int Newsgroup::get_id_nbr() const {
	return id_nbr;
}

vector<Article> Newsgroup::get_articles() const {
	return articles;
}

void Newsgroup::add_article(Article art) {
	articles.push_back(art);
}

void Newsgroup::remove_article(int id_nbr) {
    auto art_it = std::find_if( articles.begin(), articles.end(), 
	[id_nbr](const Article art){ return art.getIdNbr() == id_nbr;} );
	articles.erase(art_it);
}

