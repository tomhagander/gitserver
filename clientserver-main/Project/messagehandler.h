
#ifndef MSG_HANDLE
#define MSG_HANDLE

#include <string>
#include <vector>
#include <tuple>
#include "connection.h"

/* A class for handling low-level protocol messaging */
class Messagehandler {
    public:

        Messagehandler();

        ~Messagehandler();

        // MEMORY??
        // returns list of pairs with id number and name
        std::vector<std::pair<int, std::string> > com_list_ng(const Connection& conn);

        // returns true if successful otherwise false
        bool com_create_ng(const Connection& conn, std::string name);

        // returns true if successful otherwise false
        bool com_delete_ng(const Connection& conn, int ng_id_nbr);

        //MEMORY??
        // returns list of pairs with id nbr and title
        std::vector<std::pair<int, std::string> > com_list_art(const Connection& conn, int ng_id_nbr);

        // returns true if successful otherwise false
        bool com_create_art(const Connection& conn, int ng_id_nbr, std::string title, std::string author, std::string text);

        // returns true if successful otherwise false
        bool com_delete_art(const Connection& conn, int ng_id_nbr, int art_id_nbr);

        // returns nothing but sends references to the strings. hope this works
        void com_get_art(const Connection& conn, int ng_id_nbr, int art_id_nbr, std::string& title, std::string& author, std::string& text);

    private:
        int readNumber(const Connection& conn);

        void writeNumber(const Connection& conn, int nbr);

        int read_num_p(const Connection& conn);

        std::string read_string_p(const Connection& conn);

        void write_string_p(const Connection& conn, std::string s);

        void write_num_p(const Connection& conn, int nbr);
};

#endif