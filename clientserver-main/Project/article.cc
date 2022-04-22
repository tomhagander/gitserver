#include "article.h"

#include <string>
#include <iostream>
using std::string;

article::article(std::string& t, std::string& a, std::string& txt): title{t}, author{a}, text{txt} {}

// article::article(char* t, char* a, char* txt){
//     string t_string(t);
//     string a_string(a);
//     string txt_string(txt);
//     article(t_string, a_string, txt_string);
// }
string article::getAuthor() const{
    return author;
}

string article::getText() const{
    return text;
}

string article::getTitle() const{
    return title;
}