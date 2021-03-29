#ifndef SERVER_BASE_H
#define SERVER_BASE_H

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
    void exec(const MessageHandler &msh,
              const DBInterface &db, const Message &message)
    {
        Message ms = Message{};
        try
        {
            switch (message.getCommand())
            {
            case Protocol::COM_LIST_NG:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_LIST_ART:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_CREATE_NG:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_CREATE_ART:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_DELETE_NG:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_DELETE_ART:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            case Protocol::COM_GET_ART:
            {
                // read rest of the parameters and process
                // generate respose
                break;
            }
            default:
                break;
            }
        }
        catch (DBException &err)
        {
            // generate exception message
        }
        msh.send(ms);
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
