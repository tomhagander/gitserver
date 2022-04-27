#ifndef DATABASE_H
#define DATABASE_H

#include "article.h"
#include "newsgroup.h"
#include <string>
#include <vector> 
using std::string;
using std::vector;
using std::pair;

class database{
    public:
        database();

        ~database();

        // borde vara vector<pair<int, string>>
        vector<pair<int, string> > list_groups() const;

        // should return false if already exists
        bool create_group(string ng_name);

        // borde nog inte ta string, ska deleta på id nbr
        bool delete_group(int ng_id_nbr);

        // borde vara vector<pair<int, string>>. Borde ta int och inte string. Borde throwa does not exist, finns h-filer för dessa exceptions i include 
        vector<pair<int, string> > list_articles(int ng_id_nbr) const;

        // borde ta id nbrs istället för string. Borde throwa does not exsist
        // VAL att göra: Ska server_interface känna till article eller ska den passa referenser till read?? Implementerar referenser sålänge
        bool read(int ng_id_nbr, int art_id_nbr, string& title, string& author, string& text) const;

        // borde ta id nbr. Vad vi väljer ovan påverkar vad som skickas in till denna.
        // borde alltså ta int string string string
        bool write(int ng_id_nbr, string title, string author, string text);

        // borde inte ta string, ska vara id nbr. Borde throwa två olika sorters error beroende på vilken som inte finns
        bool delete_article(int ng_id_nbr, int art_id_nbr);
    private:
        vector<Newsgroup> groups;
        int ctr;
};



#endif