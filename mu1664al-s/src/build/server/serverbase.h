#ifndef SERVER_BASE_H
#define SERVER_BASE_H

#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "dbinterface.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using std::cerr;
using std::endl;
using std::exception;
using std::make_shared;
using std::shared_ptr;
using std::stoi;

// ATTENTION this is the provided test server.
// It will be the base of our server

/*
 * Read an integer from a server.
 */
class ServerBase
{
public:
    int readNumber(const shared_ptr<Connection> &conn)
    {
        unsigned char byte1 = conn->read();
        unsigned char byte2 = conn->read();
        unsigned char byte3 = conn->read();
        unsigned char byte4 = conn->read();
        return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    }

    /*
 * Send a string to a client.
 */
    void writeString(const shared_ptr<Connection> &conn, const string &s)
    {
        for (char c : s)
        {
            conn->write(c);
        }
        conn->write('$');
    }

    Server init(int argc, char *argv[])
    {
        if (argc != 2)
        {
            cerr << "Usage: myserver port-number" << endl;
            exit(1);
        }

        int port = -1;
        try
        {
            port = stoi(argv[1]);
        }
        catch (exception &e)
        {
            cerr << "Wrong format for port number. " << e.what() << endl;
            exit(2);
        }

        Server server(port);
        if (!server.isReady())
        {
            cerr << "Server initialization error." << endl;
            exit(3);
        }
        return server;
    }
};

#endif
