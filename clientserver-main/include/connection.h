// ------------------------------------------------------------------
//
//                                 Client/Server communication package
//
//                                               Connection header file
//
// Change log
//  950323  RH  Initial version
//  951212  RH  Modified to allow subclassing of class Connection
//  970127  RH  Changed "private" to "protected"
//  990125  PH  Changed function names: Read -> read, etc.
//  000114  PH  int -> bool, virtual destructors, other minor changes
//  010129  PH  added void type to initConnection
//  011212  PH  changed char* arguments to const char*
//              changed connection closed handling to exception
//              unsigned char instead of char/int in write/read
//  020102  PH  split into separate file for each class
//  040421  PH  added namespace, new casts, cleaned up a lot
//  050113  PH  added deregisterConnection, new registration (vector),
//              added check for server shutdown, many more changes
//  130515  PH  removed namespace
//  141118  PH  deleted copy constructor & copy assignment
//  190211  SGR refactored constructors
//              added named constant for invalid socket
//
// ------------------------------------------------------------------

#ifndef CONNECTION_H
#define CONNECTION_H

class Server;

/* A Connection object represents a connection (a socket)  */
class Connection {
        friend class Server;

      public:
        /* Establishes a connection to the computer 'host' via
           the port 'port' */
        Connection(const char* host, int port);

        /* Creates a Connection object which will be initialized
           by the server */
        Connection();

        /* Closes the connection */
        virtual ~Connection();

        /* Returns true if the connection has been established */
        bool isConnected() const;

        /* Writes a character */
        void write(unsigned char ch) const;

        /* Reads a character */
        unsigned char read() const;

        /* Connection cannot be copied or assigned */
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection& operator=(Connection&&) = delete;

        /* Connection can be move constructed */
        Connection(Connection&&);

      protected:
        static constexpr int no_socket {-1};

        /* The socket number that this connections communicates on */
        int my_socket {no_socket};

        /* Set to true when the constructor has called signal()
           to ignore broken pipe. See comment in the constructor */
        static bool ignoresPipeSignals;

        /* Initialization from server, receives socket number s */
        void initConnection(int s);

        /* Server fetches the socket number */
        int getSocket() const;

        /* Prints error message and exits */
        void error(const char* msg) const;
};

#endif
