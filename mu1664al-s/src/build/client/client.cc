#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::make_shared;
using std::stoi;
using std::string;

// ATTENTION this is the provided test client.
// It will be the base of our client

Connection init(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }

    int port = -1;
    try
    {
        port = stoi(argv[2]);
    }
    catch (exception &e)
    {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(2);
    }

    Connection conn(argv[1], port);
    if (!conn.isConnected())
    {
        cerr << "Connection attempt failed" << endl;
        exit(3);
    }

    return conn;
}

void handleCommand(const MessageHandler &msh, int command)
{
    // do the same for the rest of the commands
    switch (command)
    {
    case static_cast<int>(Protocol::COM_CREATE_NG):
    {
        // guide user through input
        // generate params
        Parameters params;
        msh.sendRequest(Protocol::COM_CREATE_NG, params);
        break;
    }
    default:
        break;
    }
    Message ms = msh.recieve();
    if (ms.status == Protocol::ANS_ACK)
    {
        switch (ms.command)
        {
        case Protocol::ANS_CREATE_NG:
        {
            // handle response data
            break;
        }
        default:
            break;
        }
    }
    else
    {
        // handle rejection
    }
}

int app(const MessageHandler &msh)
{
    // print the commands here
    cout << "Type a command (number): ";
    int command;
    while (cin >> command)
    {
        cout << endl;
        try
        {
            handleCommand(msh, command);
        }
        catch (ConnectionClosedException &)
        {
            cout << " no reply from server. Exiting." << endl;
            return 1;
        }
        // print the commands here
        cout << "Next command (number): ";
    }
    return 0;
}

int main(int argc, char *argv[])
{
    MessageHandler msh = MessageHandler(make_shared<Connection>(init(argc, argv)));
    return app(msh);
}
