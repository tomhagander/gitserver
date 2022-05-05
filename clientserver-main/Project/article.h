#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article{
    public:
        Article(std::string& t, std::string& a, std::string& txt, int id_nbr);
        std::string getAuthor() const;
        std::string getText() const;
        std::string getTitle() const;
        int getIdNbr() const;
    private:
        int id_nbr;
        std::string title;
        std::string author;
        std::string text;

};   

#endif