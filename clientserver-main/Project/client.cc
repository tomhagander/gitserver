#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <stdexcept>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value)
{
        conn.write((value >> 24) & 0xFF);
        conn.write((value >> 16) & 0xFF);
        conn.write((value >> 8) & 0xFF);
        conn.write(value & 0xFF);
}

/*
 * Send a string to a client.
 */
void writeString(const std::shared_ptr<Connection>& conn, const string& s)
{
        for (char c : s) {
                conn->write(c);
        }
        conn->write('$');
}

/*
 * Read an integer from a client.
 */
int readNumber(const std::shared_ptr<Connection>& conn)
{
        unsigned char byte1 = conn->read();
        unsigned char byte2 = conn->read();
        unsigned char byte3 = conn->read();
        unsigned char byte4 = conn->read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Read a string from the server.
 */
string readString(const Connection& conn)
{
        string s;
        char   ch;
        while ((ch = conn.read()) != '$') {
                s += ch;
        }
        return s;
}

Connection init(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Usage: myclient host-name port-number" << endl;
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
    cout << "Welcome to the newsserver! Type help for list of operations" << endl;
    cout << "Type the number of an operation: ";
    
}

int main(int argc, char* argv[]){
    Connection conn = init(argc, argv);
    return app(conn);
}