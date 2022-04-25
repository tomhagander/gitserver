
#ifndef MSG_HANDLE
#define MSG_HANDLE

#include <string>
#include <vector>
#include "connection.h"

/* A class for handling low-level protocol messaging */
class Messagehandler {
    public:

        Messagehandler();

        ~Messagehandler();

        std::vector<std::string> com_list_ng(const Connection& conn);

        void com_create_ng(const Connection& conn, std::string name);

        void com_delete_ng(const Connection& conn, int ng_id_nbr);

        void com_list_art(const Connection& conn, int ng_id_nbr);

        void com_create_art(const Connection& conn, int ng_id_nbr, std::string title, std::string author, std::string text);

        void com_delete_art(const Connection& conn, int ng_id_nbr, int art_id_nbr);

        void com_get_art(const Connection& conn, int ng_id_nbr, int art_id_nbr);




    private:

};



#endif