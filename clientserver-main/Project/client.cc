#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;



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
            cout << "listnewsgroups  - provides a list of existing newsgroups" << endl;
            cout << "createnewsgroup - creates a new newsgroup. Prompt for name will appear" << endl;
            cout << "deletenewsgroup - deletes a newsgroup. Prompt for newsgroup id number will appear" << endl;
            cout << "listarticles    - lists articles in a newsgroup. Promt for newsgroup id number will appear" << endl;
            cout << "createarticle   - creates an article. Prompt for newsgroup id number and info will appear" << endl;
            cout << "deletearticle   - deletes an article. Prompt for newsgroup and article id number will appear" << endl;
            cout << "getarticle      - gets an article. Prompt for newsgroup and article id number will appear" << endl;
            cout << "exit            - close client" << endl;

        } else if (input == "listnewsgroups"){
            vector<pair<int, string> > groups;
            try {
                groups = msg.com_list_ng(conn);
            } catch (std::exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            for (vector<pair<int, string> >::iterator itr = groups.begin(); itr != groups.end(); itr++){
                cout << itr->first << " " << itr->second << endl;
            }

        } else if (input == "createnewsgroup"){
            bool success = false;
            string prompt;
            cout << "Type the name of the new newsgroup: ";
            cin >> prompt;
            try {
                success = msg.com_create_ng(conn, prompt);
            } catch (const std::exception& e){
                cout << "Error: " << e.what() << endl;
            }
            if (success){
                cout << "Success! Created " << prompt << endl;
            }

        } else if (input == "deletenewsgroup"){
            bool success = false;
            int prompt;
            cout << "Type the id number of the newsgroup you want to delete: ";
            try {
                cin >> prompt;
            } catch (...) {
                cout << "Wrong input format!" << endl;
                break;
            }
            try {
                success = msg.com_delete_ng(conn, prompt);
            } catch (const std::exception& e){
                cout << "Error: " << e.what() << endl;
            }
            if (success){
                cout << "Success! Deleted group number " << prompt << endl;
            }

        } else if (input == "listarticles"){
            int ng_id_nbr;
            cout << "Type the id number of your desired newsgroup: ";
            try {
                cin >> ng_id_nbr;
            } catch (...) {
                cout << "Wrong input format!" << endl;
                break;
            }
            vector<pair<int, string> > articles;
            try {
                articles = msg.com_list_art(conn, ng_id_nbr);
            } catch (std::exception& e) {
                cout << "Error: " << e.what() << endl;
            } for (vector<pair<int, string> >::iterator itr = articles.begin(); itr != articles.end(); itr++){
                cout << itr->first << " " << itr->second << endl;
            }

        } else if (input == "createarticle"){
            bool success = false;
            int ng_id_nbr;
            string title;
            string author;
            string text;
            try {
                cout << "Type the id number of your desired newsgroup: ";
                cin >> ng_id_nbr;
            } catch (...) {
                cout << "Wrong input format! " << endl;
            }
            cout << "Article title: ";
            cin >> title;
            cout << "Article author: ";
            cin >> author;
            cout << "Article text: ";
            cin >> text;

            try {
                success = msg.com_create_art(conn, ng_id_nbr, title, author, text);
            } catch (std::exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            if (success) {
                cout << "Success! Created article " << title << endl;
            }

        } else if (input == "deletearticle"){
            bool success = false;
            int ng_id_nbr;
            int art_id_nbr;
            try {
                cout << "Type the id number of the article's newsgroup: ";
                cin >> ng_id_nbr;
                cout << "Type the id number of the article to be deleted: ";
                cin >> art_id_nbr;
            } catch (...) {
                cout << "Wrong input format! " << endl;
            }

            try {
                success = msg.com_delete_art(conn, ng_id_nbr, art_id_nbr);
            } catch (std::exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            if (success) {
                cout << "Success! Deleted article." << endl;
            }

        } else if (input == "getarticle"){
            string title;
            string author;
            string text;
            int ng_id_nbr;
            int art_id_nbr;
            try {
                cout << "Type the id number of the article's newsgroup: ";
                cin >> ng_id_nbr;
                cout << "Type the id number of the article: ";
                cin >> art_id_nbr;
            } catch (...) {
                cout << "Wrong input format! " << endl;
            }

            try {
                msg.com_get_art(conn, ng_id_nbr, art_id_nbr, title, author, text);
            } catch (std::exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            cout << title << endl;
            cout << author << endl;
            cout << text << endl;
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