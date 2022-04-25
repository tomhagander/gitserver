#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class article{
    public:
        article(std::string& t, std::string& a, std::string& txt);
        // article(char* t, char* a, char* txt);
        std::string getAuthor() const;
        std::string getText() const;
        std::string getTitle() const;
    private:
        std::string title;
        std::string author;
        std::string text;

};   

#endif