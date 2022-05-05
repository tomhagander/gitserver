#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "protocol.h"
#include "database.h"
#include "servermsghandler.h"


#include "clientmisbehavedexception.h"
#include "badartnumber.h"
#include "badngnumber.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::vector;

void process_request(std::shared_ptr<Connection>& conn, database& db, Servermessagehandler& smsg) {
    unsigned char com = conn->read();

    if (com == static_cast<int>(Protocol::COM_LIST_NG)){

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }


        conn->write(static_cast<int>(Protocol::ANS_LIST_NG));
        smsg.send_list(conn, db.list_groups());
        conn->write(static_cast<int>(Protocol::ANS_END));

    } 

    else if (com == static_cast<int>(Protocol::COM_CREATE_NG)) {
        string name = smsg.read_string_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }
        
        bool success;
        try{
            success = db.create_group(name);
        } catch (...){
            throw std::runtime_error("Unknown server error");
        }

        conn->write(static_cast<int>(Protocol::ANS_CREATE_NG));
        if (success){
            conn->write(static_cast<int>(Protocol::ANS_ACK));
        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    }

    else if (com == static_cast<int>(Protocol::COM_DELETE_NG)){
        int ng_id_nbr = smsg.read_num_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }

        bool success;
        try {
            success = db.delete_group(ng_id_nbr);
        } catch (...) {
            throw std::runtime_error("Unknown server error");
        }

        conn->write(static_cast<int>(Protocol::ANS_DELETE_NG));
        if (success){
            conn->write(static_cast<int>(Protocol::ANS_ACK));
        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    }

    else if (com == static_cast<int>(Protocol::COM_LIST_ART)) {
        int ng_id_nbr = smsg.read_num_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }

        bool success;
        vector<pair<int, string> > articles;
        try {
            articles = db.list_articles(ng_id_nbr);
            success = true;
        } catch (...){ 
            success = false;
        }

        conn->write(static_cast<int>(Protocol::ANS_LIST_ART));
        if (success) {
            conn->write(static_cast<int>(Protocol::ANS_ACK));
            smsg.send_list(conn, articles);
        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    }

    else if (com == static_cast<int>(Protocol::COM_CREATE_ART)){
        int ng_id_nbr = smsg.read_num_p(conn);
        string title = smsg.read_string_p(conn);
        string author = smsg.read_string_p(conn);
        string text = smsg.read_string_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }

        bool success;
        try{
            success = db.write(ng_id_nbr, title, author, text);
        } catch (BadNGException& e) {
            success = false;
        } catch (BadARTException& e){
            success = false;
        }

        conn->write(static_cast<int>(Protocol::ANS_CREATE_ART));
        if (success) {
            conn->write(static_cast<int>(Protocol::ANS_ACK));
        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    }

    else if (com == static_cast<int>(Protocol::COM_DELETE_ART)){
        int ng_id_nbr = smsg.read_num_p(conn);
        int art_id_nbr = smsg.read_num_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }

        bool success;
        Protocol err_type;
        try {
            db.delete_article(ng_id_nbr, art_id_nbr);
            success = true;
        } catch (BadNGException& e){
            success = false;
            err_type = Protocol::ERR_NG_DOES_NOT_EXIST;
        } catch (BadARTException& e){
            success = false;
            err_type = Protocol::ERR_ART_DOES_NOT_EXIST;
        } catch (...){
            throw std::runtime_error("Unknown server error");
        }

        conn->write(static_cast<int>(Protocol::ANS_DELETE_ART));
        if (success) {
            conn->write(static_cast<int>(Protocol::ANS_ACK));
        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(err_type));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    }
    else if (com == static_cast<int>(Protocol::COM_GET_ART)){
        int ng_id_nbr = smsg.read_num_p(conn);
        int art_id_nbr = smsg.read_num_p(conn);

        unsigned char com_end = conn->read();
        if (com_end != static_cast<int>(Protocol::COM_END)){
            throw ClientMisbehavedException();
        }

        string title;
        string author; 
        string text;

        bool success;
        Protocol err_type;
        try {
            db.read(ng_id_nbr, art_id_nbr, title, author, text);
            success = true;
        } catch (BadNGException& e){
            success = false;
            err_type = Protocol::ERR_NG_DOES_NOT_EXIST;
        } catch (BadARTException& e){
            success = false;
            err_type = Protocol::ERR_ART_DOES_NOT_EXIST;
        } catch (...){
            throw std::runtime_error("Unknown server error");
        }

        conn->write(static_cast<int>(Protocol::ANS_GET_ART));
        if (success) {
            conn->write(static_cast<int>(Protocol::ANS_ACK));
            smsg.send_string_p(conn, title);
            smsg.send_string_p(conn, author);
            smsg.send_string_p(conn, text);

        } else {
            conn->write(static_cast<int>(Protocol::ANS_NAK));
            conn->write(static_cast<int>(err_type));
        }
        conn->write(static_cast<int>(Protocol::ANS_END));
    } else {
        throw ClientMisbehavedException();
    }
}

void serve_one(Server& server, database& db, Servermessagehandler& smsg) {
    auto conn = server.waitForActivity();
    if (conn != nullptr) {
        try {
            process_request(conn, db, smsg);
        } catch (ConnectionClosedException&) {
            server.deregisterConnection(conn);
            cout << "Client closed connection" << endl;
        }
    } else {
        conn = std::make_shared<Connection>();
        server.registerConnection(conn);
        cout << "New client connected" << endl;
    }
}

Server init(int argc, char* argv[]) {
    if (argc != 2) {
                cerr << "Usage: myserver port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = std::stoi(argv[1]);
        } catch (std::exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }
        return server;
}

int main(int argc, char* argv[]) {
    auto server = init(argc, argv);

    database db{};

    Servermessagehandler smsg{};

    while(true) {
        serve_one(server, db, smsg);    
    }
    return 0;
}