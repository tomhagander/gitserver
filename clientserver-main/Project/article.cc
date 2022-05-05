#include "article.h"

#include <string>
#include <iostream>
using std::string;

string Article::getAuthor() const{
    return author;
}

string Article::getText() const{
    return text;
}

string Article::getTitle() const{
    return title;
}

int Article::getIdNbr() const {
	return id_nbr;
}