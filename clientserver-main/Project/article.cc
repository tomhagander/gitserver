#include "article.h"

#include <string>
#include <iostream>
using std::string;

Article::Article(std::string& t, std::string& a, std::string& txt, int id): title{t}, author{a}, text{txt} id_nbr{id} {}

// article::article(char* t, char* a, char* txt){
//     string t_string(t);
//     string a_string(a);
//     string txt_string(txt);
//     article(t_string, a_string, txt_string);
// }
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