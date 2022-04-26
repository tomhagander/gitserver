#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <iostream>
#include <stdexcept>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;



Connection init(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Usage: client host-name port-number" << endl;
        exit(1);
    }

    int port = -1;
    try {
        port = std::stoi(argv[2]);
    } catch (std::exception& e) {
        std::cerr << "Wrong port number. " << e.what() << endl;
        exit(2);
    }

    Connection conn(argv[1], port);
    if (!conn.isConnected()) {
        std::cerr << "Connection attempt failed" << endl;
        exit(3);
    }

    return conn;
}

int app(const Connection& conn){
    Messagehandler msg{};
    cout << "Welcome to the newsserver! Type help for a list of operations" << endl;
    cout << "Type an operation: ";
    string input;
    while(cin >> input){
        if (input == "help"){
            cout << "Commands: " << endl;
            cout << "listnewsgroups - provides a list of existing newsgroups" << endl;
            cout << "createnewsgroup - creates a new newsgroup. Prompt for name will appear" << endl;
            cout << "deletenewsgroup - deletes a newsgroup. Prompt for newsgroup id number will appear" << endl;
            cout << "listarticles - lists articles in a newsgroup. Promt for newsgroup id number will appear" << endl;
            cout << "createarticle - creates an article. Prompt for newsgroup id number and info will appear" << endl;
            cout << "deletearticle - deletes an article. Prompt for newgroup and article id number will appear" << endl;
            cout << "getarticle - gets an article. Prompt for newgroup and article id number will appear" << endl;
            cout << "exit - close client" << endl;

        } else if (input == "listnewsgroups"){
            cout << "1" << endl;
        } else if (input == "createnewsgroup"){
            cout << "2" << endl;
        } else if (input == "deletenewsgroup"){
            cout << "3" << endl;
        } else if (input == "listarticles"){
            cout << "4" << endl;
        } else if (input == "createarticle"){
            cout << "5" << endl;
        } else if (input == "deletearticle"){
            cout << "6" << endl;
        } else if (input == "getarticle"){
            cout << "7" << endl;
        } else if (input == "exit"){
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Input not recognized!" << endl;
        }
        cout << endl;
        cout << "Type an operation: ";
    }
    return 0;
}

int main(int argc, char* argv[]){
    Connection conn = init(argc, argv);
    return app(conn);
}