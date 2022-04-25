#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "protocol.h"

#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;


Messagehandler::Messagehandler() = default;

Messagehandler::~Messagehandler() = default;

int Messagehandler::readNumber(const Connection& conn){
    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int Messagehandler::read_num_p(const Connection& conn){
    unsigned char par_type = conn.read();
    if (par_type != Protocol::PAR_NUM){
        //throw some shit
    }
    return readNumber(conn);
}

// reads string starting from number of bytes coming
string Messagehandler::read_string_p(const Connection& conn){
    unsigned char par_type = conn.read();
    if (par_type != Protocol::PAR_STRING){
        //throw some shit
    }
    int num_chars = readNumber(conn);
    string s;
    char ch;
    for (int i = 0; i < num_chars; i++){
        ch = conn.read();
        s += ch;
    }
    return s;
}

void Messagehandler::writeNumber(const Connection& conn, int nbr){
    conn.write((nbr >> 24) & 0xFF);
    conn.write((nbr >> 16) & 0xFF);
    conn.write((nbr >> 8) & 0xFF);
    conn.write(nbr & 0xFF);
}

void Messagehandler::write_num_p(const Connection& conn, int nbr){
    conn.write(Protocol::PAR_NUM);
    writeNumber(conn, nbr);
}

void Messagehandler::write_string_p(const Connection& conn, string s){
    conn.write(Protocol::PAR_STRING);
    writeNumber(conn, s.length());
    for (auto ch : s){
        conn.write(ch);
    }
}

vector<pair<int, string> > Messagehandler::com_list_ng(const Connection& conn){
    // writing command
    conn.write(Protocol::COM_LIST_NG);
    conn.write(Protocol::COM_END);

    // reading answer
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_LIST_NG){
        // throw some shit får se hur det borde lösas
    }
    int nbr_ng = readNumber(conn);
    vector<pair<int, string> > groups(nbr_ng);
    for (vector<pair<int, string> >::iterator itr = groups.begin(); itr != groups.end(); itr++){
        int id_num = read_num_p(conn);
        string name = read_string_p(conn);
        *itr = pair<int, string>(id_num, name);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        // throw some shit får se hur det borde lösas
    }
    return groups;
}

bool Messagehandler::com_create_ng(const Connection& conn, string name){
    // writing command
    conn.write(Protocol::COM_CREATE_NG);
    write_string_p(conn, name);
    conn.write(Protocol::COM_END);

    // reading answer
    bool success;
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_CREATE_NG){
        // throw some shit
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        // throwa error här??
    } else {
        // throw some shit
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        // throw some shit får se hur det borde lösas
    }
    return success;

}

bool Messagehandler::com_delete_ng(const Connection& conn, int id_nbr){
    // writing command
    conn.write(Protocol::COM_DELETE_NG);
    write_num_p(conn, id_nbr);
    conn.write(Protocol::COM_END);

    // reading answer
    bool success;
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_DELETE_NG){
        // throw some shit
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        // throwa error här??
    } else {
        // throw some shit
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        // throw some shit får se hur det borde lösas
    }
    return success;
}

