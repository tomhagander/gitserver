// ------------------------------------------------------------------
//
//                                 Client/Server communication package
//
//                                         Connection implementation file
//
// Change log
//  950323  RH  Initial version
//  951212  RH  Modified to allow subclassing of class Connection
//  970127  RH  Added extra include to make the file compile under Linux
//  990125  PH  Changed function names: Read -> read, ...
//  000114  PH  int -> bool, virtual destructors, other minor changes
//  010129  PH  added void type to initConnection
//  011212  PH  changed char* arguments to const char*
//              changed connection closed handling to exception
//              unsigned char instead of char/int in write/read
//  020102  PH  split into separate file for each class
//  040421  PH  added namespace, new casts, cleaned up a lot
//  050113  PH  added deregisterConnection, new registration (vector),
//              added check for server shutdown, many more changes
//  090127  PH  added include of cstdlib, for exit()
//  130515  PH  removed namespace
//  190211  SGR refactored constructors
//              added named constant for invalid socket
//
// ------------------------------------------------------------------

#include "connection.h"

#include "connectionclosedexception.h"

#include <arpa/inet.h> /* htons() */
#include <csignal>     /* signal() */
#include <cstdlib>     /* exit() */
#include <cstring>     /* memcpy() */
#include <iostream>
#include <netdb.h>      /* gethostbyname() */
#include <netinet/in.h> /* sockaddr_in */
#include <sys/socket.h> /* socket(), connect() */
#include <sys/types.h>  /* socket(), connect(), read(), write() */
#include <sys/uio.h>    /* read(), write() */
#include <unistd.h>     /* close(), read(), write() */

bool Connection::ignoresPipeSignals = false;

Connection::Connection()
{
        /*
         * Ignore SIGPIPE signals (broken pipe). A broken pipe (only?)
         * occurs in a client, when it tries to write to a dead server.
         * When the signal is ignored, ::write() returns -1 as the count
         * of written bytes. Connection::write() tests for this and
         * throws ConnectionClosedException when it happens.
         */
        if (!ignoresPipeSignals) {
                signal(SIGPIPE, SIG_IGN);
                ignoresPipeSignals = true;
        }
}

Connection::Connection(const char* host, int port) : Connection()
{
        my_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (my_socket < 0) {
                my_socket = no_socket;
                return;
        }

        sockaddr_in server;
        server.sin_family = AF_INET;
        hostent* hp       = gethostbyname(host);
        if (hp == 0) {
                my_socket = no_socket;
                return;
        }

        memcpy(reinterpret_cast<char*>(&server.sin_addr),
               reinterpret_cast<char*>(hp->h_addr), hp->h_length);
        server.sin_port = htons(port);
        if (connect(my_socket, reinterpret_cast<sockaddr*>(&server),
                    sizeof(server)) < 0) {
                my_socket = no_socket;
        }
}

Connection::Connection(Connection&& o) : my_socket {o.my_socket}
{
        o.my_socket = no_socket;
}

Connection::~Connection()
{
        if (my_socket != no_socket) {
                close(my_socket);
        }
}

bool Connection::isConnected() const { return my_socket != no_socket; }

void Connection::write(unsigned char ch) const
{
        if (my_socket == no_socket) {
                error("Write attempted on a not properly opened connection");
        }
        int count = ::write(my_socket, &ch, 1);
        if (count != 1) {
                throw ConnectionClosedException();
        }
}

unsigned char Connection::read() const
{
        if (my_socket == no_socket) {
                error("Read attempted on a not properly opened connection");
        }
        char data;
        int  count = ::read(my_socket, &data, 1);
        if (count != 1) {
                throw ConnectionClosedException();
        }
        return data;
}

void Connection::initConnection(int s) { my_socket = s; }

int Connection::getSocket() const { return my_socket; }

void Connection::error(const char* msg) const
{
        std::cerr << "Class Connection: " << msg << std::endl;
        exit(1);
}
