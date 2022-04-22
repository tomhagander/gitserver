#include "article.h"

#include <string>
#include <iostream>
using std::string;

article::article(std::string& t, std::string& a, std::string& txt): title{t}, author{a}, text{txt} {}

string article::getAuthor() const{
    return author;
}

string article::getText() const{
    return text;
}

string article::getTitle() const{
    return title;
}