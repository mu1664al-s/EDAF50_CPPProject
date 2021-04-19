#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <ios>
#include <stdexcept>
#include <string>
#include <memory>
#include <ios>
#include <limits>

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
                cerr << "Usage: client host-name port-number" << endl;
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

void app(shared_ptr<Connection> conn, Message &ms)
{
        // print the commands here
        try
        {
                MessageHandler::send(conn, ms);
                MessageHandler::recieve(conn);
        }
        catch (ConnectionClosedException &)
        {
                cout << "No reply from server. Exiting." << endl;
        }
}

int main(int argc, char *argv[])
{

        cout << "illegal status: COM_LIST ANS_ACK" << endl;
        Message ms = Message();
        ms.setCommand(Protocol::COM_LIST_NG).setStatus(Protocol::ANS_ACK);
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal parameters: COM_LIST PAR_NUM PAR_STRING" << endl;
        ms = Message();
        ms.setCommand(Protocol::COM_LIST_NG).addNumParam(2).addStrParam("haha");
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal parameter: COM_CREATE_NG PAR_NUM" << endl;
        ms = Message();
        ms.setCommand(Protocol::COM_CREATE_NG).addNumParam(3);
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal and missing parameter: COM_DELETE_ART PAR_STRING" << endl;
        ms = Message();
        ms.setCommand(Protocol::COM_DELETE_ART).addStrParam("haha");
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal command with illegal and missing parameters: ANS_CREATE_ART PAR_STRING PAR_NUM" << endl;
        ms = Message();
        ms.setCommand(Protocol::ANS_CREATE_ART).addStrParam("haha").addNumParam(5);
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal and missing parameters: COM_GET_ART PAR_STRING" << endl;
        ms = Message();
        ms.setCommand(Protocol::COM_GET_ART).addStrParam("test");
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "illegal status: COM_GET_ART ANS_NAK" << endl;
        ms = Message();
        ms.setCommand(Protocol::COM_GET_ART).setStatus(Protocol::ANS_NAK);
        app(make_shared<Connection>(init(argc, argv)), ms);

        cout << "completely illegal: PAR_NUM PAR_STRING" << endl;
        ms = Message();
        ms.addNumParam(3).addStrParam("test");
        app(make_shared<Connection>(init(argc, argv)), ms);
        return 0;
}
