
#ifndef SERVER_MSG_HANDLE
#define SERVER_MSG_HANDLE

#include <string>
#include <vector>
#include "connection.h"
#include "protocol.h"

using std::vector;
using std::pair;
using std::string;

class Servermessagehandler {
    public:
        Servermessagehandler();

        ~Servermessagehandler();

        // sends list on format num_p [num_p string_p]*
        void send_list(const std::shared_ptr<Connection>& conn, vector<pair<int, string> > list);

        // sends num_p
        void send_num_p(const std::shared_ptr<Connection>& conn, int nbr);

        // sends string_p
        void send_string_p(const std::shared_ptr<Connection>& conn, string s);

        int read_num_p(const std::shared_ptr<Connection>& conn);

        std::string read_string_p(const std::shared_ptr<Connection>& conn);

    private:
        void writeNumber(const std::shared_ptr<Connection>& conn, int value);

        int readNumber(const std::shared_ptr<Connection>& conn);
};

#endif