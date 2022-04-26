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


// är det okej med default?? idk?
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
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    
    int nbr_ng = read_num_p(conn);
    vector<pair<int, string> > groups(nbr_ng);

    // bra med try?? I DUNNO
    try {
        for (vector<pair<int, string> >::iterator itr = groups.begin(); itr != groups.end(); itr++){
            int id_num = read_num_p(conn);
            string name = read_string_p(conn);
            *itr = pair<int, string>(id_num, name);
        }
    } catch (std::exception& e) {
        throw std::runtime_error("Something wrong from server");
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
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
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        throw std::runtime_error("Ans not accepted");
    } else {
        throw std::runtime_error("Wierd answer: " + acc_status);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
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
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        throw std::runtime_error("Ans not accepted");
    } else {
        throw std::runtime_error("Wierd answer: " + acc_status);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
    }
    return success;
}

vector<pair<int, string> > Messagehandler::com_list_art(const Connection& conn, int ng_id_nbr){
    // writing command
    conn.write(Protocol::COM_LIST_ART);
    write_num_p(conn, ng_id_nbr);
    conn.write(Protocol::COM_END);

    // reading answer
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_LIST_ART){
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char ans_acc = conn.read();
    if (ans_acc == Protocol::ANS_ACK){
        int nbr_ng = read_num_p(conn);
        vector<pair<int, string> > articles(nbr_ng);
        for (vector<pair<int, string> >::iterator itr = articles.begin(); itr != articles.end(); itr++){
            int id_num = read_num_p(conn);
            string name = read_string_p(conn);
            *itr = pair<int, string>(id_num, name);
        }
        unsigned char ans_end = conn.read();
        if (ans != Protocol::ANS_END){
            throw std::runtime_error("Answer not terminated");
        }
        return articles;
    } else if (ans_acc == Protocol::ANS_NAK) {
        unsigned char err_msg = conn.read();
        throw std::runtime_error("Not accepted: " + err_msg);
    } else {
        throw std::runtime_error("Wierd answer: " + ans_acc);
    }
    
}

bool Messagehandler::com_create_art(const Connection& conn, int ng_id_nbr, string title, string author, string text){
    // writing commmand
    conn.write(Protocol::COM_CREATE_ART);
    write_num_p(conn, ng_id_nbr);
    write_string_p(conn, title);
    write_string_p(conn, author);
    write_string_p(conn, text);
    conn.write(Protocol::COM_END);

    // reading answer
    bool success;
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_CREATE_ART){
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        throw std::runtime_error("Ans not accepted");
        // något mer här?? NAK TYPE?
    } else {
        throw std::runtime_error("Wierd answer: " + acc_status);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
    }
    return success;
}

bool Messagehandler::com_delete_art(const Connection& conn, int ng_id_nbr, int art_id_nbr){
    // writing command
    conn.write(Protocol::COM_DELETE_ART);
    write_num_p(conn, ng_id_nbr);
    write_num_p(conn, art_id_nbr);
    conn.write(Protocol::COM_END);

    // reading answer
    bool success;
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_DELETE_ART){
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char acc_status = conn.read();
    if (acc_status == Protocol::ANS_ACK){
        success = true;
    } else if (acc_status == Protocol::ANS_NAK){
        success = false;
        unsigned char err_type = conn.read();
        if (err_type == Protocol::ERR_NG_DOES_NOT_EXIST){
            throw std::runtime_error("Ans not accepted: ng does not exist");
        } else if (err_type == Protocol::ERR_ART_DOES_NOT_EXIST){
            throw std::runtime_error("Ans not accepted: article does not exist");
        } else {
            throw std::runtime_error("Wierd answer: " + err_type);
        }
    } else {
        throw std::runtime_error("Wierd answer: " + acc_status);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
    }
    return success;
}


string Messagehandler::com_get_art(const Connection& conn, int ng_id_nbr, int art_id_nbr){
    // writing command
    conn.write(Protocol::COM_GET_ART);
    write_num_p(conn, ng_id_nbr);
    write_num_p(conn, art_id_nbr);
    conn.write(Protocol::COM_END);

    // reading answer
    string title;
    string author;
    string text;
    unsigned char ans = conn.read();
    if (ans != Protocol::ANS_GET_ART){
        throw std::runtime_error("Wrong answer type or no answer: " + ans);
    }
    unsigned char ans_status = conn.read();
    if (ans_status == Protocol::ANS_ACK){
        title = read_string_p(conn);
        author = read_string_p(conn);
        text = read_string_p(conn);
    } else if (ans_status == Protocol::ANS_NAK){
        unsigned char err_type = conn.read();
        if (err_type == Protocol::ERR_NG_DOES_NOT_EXIST){
            throw std::runtime_error("Ans not accepted: ng does not exist");
        } else if (err_type == Protocol::ERR_ART_DOES_NOT_EXIST){
            throw std::runtime_error("Ans not accepted: article does not exist");
        } else {
            throw std::runtime_error("Wierd answer: " + err_type);
        }
    } else {
        throw std::runtime_error("Wierd answer: " + ans_status);
    }
    unsigned char ans_end = conn.read();
    if (ans != Protocol::ANS_END){
        throw std::runtime_error("Answer not terminated");
    }

    return; //something

}

