// ------------------------------------------------------------------
//
//                                 Client/Server communication package
//
//                                          Server implementation file
//
// Change log
// 950323  RH  Initial version
// 951212  RH  Modified to allow subclassing of class Connection
// 970127  RH  Added extra include to make the file compile under Linux
// 990125  PH  Changed function names: Read -> read, ...
// 000114  PH  int -> bool, virtual destructors, other minor changes
// 010129  PH  added void type to initConnection
// 011212  PH  changed char* arguments to const char*
//             changed connection closed handling to exception
//             unsigned char instead of char/int in write/read
// 020102  PH  split into separate file for each class
// 040421  PH  added namespace, new casts, cleaned up a lot
// 050113  PH  added deregisterConnection, new registration (vector),
//             added check for server shutdown, many more changes
// 130521  PH  removed namespace, shared pointers instead of raw
//             pointers, uses library algorithms
// 190211  SGR use Connection::no_socket instead of literal -1
//             added move ctor to Connection and Server
// ------------------------------------------------------------------

#include "server.h"

#include "connection.h"

#include <algorithm>
#include <arpa/inet.h> /* htons(), ntohs() */
#include <iostream>
#include <memory>
#include <netinet/in.h> /* sockaddr_in */
#include <sys/socket.h> /* socket(), bind(), getsockname(), listen() */
#include <sys/time.h>   /* select() */
#include <sys/types.h>  /* socket(), bind(), select() */
#include <unistd.h>     /* close(), select() */

Server::Server(int port)
{
        my_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (my_socket < 0) {
                my_socket = Connection::no_socket;
                return;
        }

        sockaddr_in server;
        server.sin_family      = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port        = htons(port);
        if (::bind(my_socket, reinterpret_cast<sockaddr*>(&server),
                   sizeof(server)) < 0) {
                my_socket = Connection::no_socket;
                return;
        }

        int length = sizeof(server);
        if (getsockname(my_socket, reinterpret_cast<sockaddr*>(&server),
                        reinterpret_cast<socklen_t*>(&length)) < 0) {
                my_socket = Connection::no_socket;
                return;
        }

        if (ntohs(server.sin_port) != port) {
                my_socket = Connection::no_socket;
                return;
        }

        listen(my_socket, 5);
}

Server::~Server()
{
        if (my_socket != Connection::no_socket) {
                close(my_socket);
        }
        my_socket = Connection::no_socket;
}

bool Server::isReady() const { return my_socket != Connection::no_socket; }

std::shared_ptr<Connection> Server::waitForActivity() const
{
        if (my_socket == Connection::no_socket) {
                error("waitForActivity: server not opened");
        }

        fd_set read_template;
        FD_ZERO(&read_template);
        FD_SET(my_socket, &read_template);
        for (const auto& conn : connections) {
                FD_SET(conn->getSocket(), &read_template);
        }

        std::shared_ptr<Connection> return_conn;
        select(FD_SETSIZE, &read_template, 0, 0, 0);
        if (FD_ISSET(my_socket, &read_template)) {
                int new_socket = accept(my_socket, 0, 0);
                if (new_socket == -1) {
                        error("waitForActivity: accept returned error");
                }
                if (pending_socket != Connection::no_socket) {
                        error("waitForActivity: a previous connection is waiting to be registered");
                }
                pending_socket = new_socket;
                return_conn    = nullptr;
        } else {
                auto it = find_if(connections.begin(), connections.end(),
                                  [&](const std::shared_ptr<Connection>& conn) {
                                          return FD_ISSET(conn->getSocket(),
                                                          &read_template);
                                  });
                if (it == connections.end()) {
                        error("waitForActivity: could not find registered connection");
                }
                return_conn = *it;
        }
        return return_conn;
}

void Server::registerConnection(const std::shared_ptr<Connection>& conn)
{
        if (conn->getSocket() != Connection::no_socket) {
                error("registerConnection: connection is busy");
        }
        if (pending_socket == Connection::no_socket) {
                error("registerConnection: no client is trying to connect");
        }
        conn->initConnection(pending_socket);
        connections.push_back(conn);
        pending_socket = Connection::no_socket;
}

void Server::deregisterConnection(const std::shared_ptr<Connection>& conn)
{
        connections.erase(
            std::remove(connections.begin(), connections.end(), conn),
            connections.end());
}

void Server::error(const char* msg) const
{
        std::cerr << "Class Server::" << msg << std::endl;
        exit(1);
}
