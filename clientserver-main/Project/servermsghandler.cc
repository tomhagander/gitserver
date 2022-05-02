#include "servermsghandler.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include "clientmisbehavedexception.h"

#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;

Servermessagehandler::Servermessagehandler() = default;

Servermessagehandler::~Servermessagehandler() = default;

void Servermessagehandler::writeNumber(const std::shared_ptr<Connection>& conn, int value)
{
        conn->write((value >> 24) & 0xFF);
        conn->write((value >> 16) & 0xFF);
        conn->write((value >> 8) & 0xFF);
        conn->write(value & 0xFF);
}

int Servermessagehandler::readNumber(const std::shared_ptr<Connection>& conn)
{
        unsigned char byte1 = conn->read();
        unsigned char byte2 = conn->read();
        unsigned char byte3 = conn->read();
        unsigned char byte4 = conn->read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void Servermessagehandler::send_num_p(const std::shared_ptr<Connection>& conn, int nbr){
    conn->write(static_cast<int>(Protocol::PAR_NUM));
    writeNumber(conn, nbr);
}

void Servermessagehandler::send_string_p(const std::shared_ptr<Connection>& conn, string s){
    conn->write(static_cast<int>(Protocol::PAR_STRING));
    writeNumber(conn, s.length());
    for (char c : s) {
        conn->write(c);
    }
}

void Servermessagehandler::send_list(const std::shared_ptr<Connection>& conn, vector<pair<int, string> > list){
    send_num_p(conn, list.size());
    for (auto itr = list.begin(); itr != list.end(); itr++){
        send_num_p(conn, itr->first);
        send_string_p(conn, itr->second);
    }
}

int Servermessagehandler::read_num_p(const std::shared_ptr<Connection>& conn){
    unsigned char par_type = conn->read();
    if (par_type != static_cast<int>(Protocol::PAR_NUM)){
        throw ClientMisbehavedException();
    }
    return readNumber(conn);
}

// reads string starting from number of bytes coming
string Servermessagehandler::read_string_p(const std::shared_ptr<Connection>& conn){
    unsigned char par_type = conn->read();
    if (par_type != static_cast<int>(Protocol::PAR_STRING)){
        throw ClientMisbehavedException();
    }
    int num_chars = readNumber(conn);
    string s;
    char ch;
    for (int i = 0; i < num_chars; i++){
        ch = conn->read();
        s += ch;
    }
    return s;
}