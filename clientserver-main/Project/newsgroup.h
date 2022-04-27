#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include "article.h"

#include <vector>

using std::string;
using std::vector;

class Newsgroup {

	public:
		Newsgroup(int ng_id_nbr, string title);
		int get_id_nbr() const;
		string get_title() const;
		vector<Article> get_articles() const;
		void add_article(Article art);
		void remove_article(int ng_id_nbr);
	private:
		string title;
		int id_nbr;
		vector<Article> articles;
};

#endif