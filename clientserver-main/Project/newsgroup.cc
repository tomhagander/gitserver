#include "newsgroup.h"
#include "article.h"

using std::string;
using std::vector;

Newsgroup::Newsgroup(int ng_id_nbr, string title) : this->title{title}, this->id_nbr{ng_id_nbr} {}

Newsgroup::get_title() const {
	return title;
}

Newsgroup::get_id_nbr() const {
	return id_nbr;
}

Newsgroup::get_articles() const {
	return articles()
}

Newsgroup::add_article(Article art) {
	articles.pushback(art);
}

Newsgroup::remove_article(int id_nbr) {
	for (Article a : articles) {
		if(a.get_id_nbr() == id_nbr) {
			articles.erase(&a);
			return;
		}
	}
}

