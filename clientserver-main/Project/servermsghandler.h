
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

        void send_list(const std::shared_ptr<Connection>& conn, vector<pair<int, string> > list);

        void send_num_p(const std::shared_ptr<Connection>& conn, int nbr);

        void send_string_p(const std::shared_ptr<Connection>& conn, string s);

        int read_num_p(const std::shared_ptr<Connection>& conn);

        std::string read_string_p(const std::shared_ptr<Connection>& conn);

    private:
        void writeNumber(const std::shared_ptr<Connection>& conn, int value);

        int readNumber(const std::shared_ptr<Connection>& conn);
};

#endif