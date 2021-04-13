#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
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

void print_commands()
{
    cout << "1. List newsgroups"
         << "\t";
    cout << "2. New newsgroup"
         << "\t";
    cout << "3. Delete newsgroup"
         << "\t";
    cout << "4. List articles"
         << endl;
    cout << "5. New article"
         << "\t";
    cout << "6. Delete article"
         << "\t";
    cout << "7. Get article"
         << "\t";
    cout << "8. Exit" << endl;
}

void listGroups(Message &ms)
{
    ms.setCommand(Protocol::COM_LIST_NG);
    cout << endl;
}

void ansListGroups(const Message &ms)
{
    try
    {
        const vector<Parameter> &params = ms.getParmaters();
        if (params[0].N > 0)
        {
            cout << ">>";
            for (int i = 1; i < params.size();)
            {
                cout << params[i].N << ". ";
                cout << params[i + 1].str << "\t";
                i += 2;
            }
        }
        else
        {
            cout << ">>No news groups found!";
        }
        cout << endl;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE};
    }
}

void listArticles(Message &ms)
{
    // guided process
}

void ansListArticles(const Message &ms)
{
    // print response
}

void createGroup(Message &ms)
{
    string title;
    cout << "Name the new group: ";
    cin >> title;
    ms.setCommand(Protocol::COM_CREATE_NG).addStrParam(title);
    cout << endl;
}

void ansCreateGroup(const Message &ms)
{
    if (ms.getStatus() == Protocol::ANS_ACK)
    {
        cout << ">>News group created." << endl;
    }
    else if (ms.getStatus() == Protocol::ANS_NAK)
    {
        cout << ">>Error! News group alredy exists." << endl;
    }
    else
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE};
    }
}

void createArticle(Message &ms)
{
    // guided process
}

void ansCreateArticle(const Message &ms)
{
    // print response
}

void deleteGroup(Message &ms)
{
    // guided process
}

void ansDeleteGroup(const Message &ms)
{
    // print response
}

void deleteArticle(Message &ms)
{
    // guided process
}

void ansDeleteArticle(const Message &ms)
{
    // print response
}

void getArticle(Message &ms)
{
    // guided process
}

void ansGetArticle(const Message &ms)
{
    // print response
}

void handleCommand(shared_ptr<Connection> conn, int command)
{
    if (command == 8)
    {
        cout << endl;
        cout << ">>Exiting..." << endl;
        exit(0);
    }
    if (command == 0)
    {
        cout << endl;
        cout << ">>Error! Please input a number between 1 and 8." << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    Message ms = Message();
    // guide the user through the process of sending these commands
    switch (static_cast<Protocol>(command))
    {
    case Protocol::COM_LIST_NG:
    {
        listGroups(ms);
        break;
    }
    case Protocol::COM_LIST_ART:
    {
        listArticles(ms);
        break;
    }
    case Protocol::COM_CREATE_NG:
    {
        createGroup(ms);
        break;
    }
    case Protocol::COM_CREATE_ART:
    {
        createArticle(ms);
        break;
    }
    case Protocol::COM_DELETE_NG:
    {
        deleteGroup(ms);
        break;
    }
    case Protocol::COM_DELETE_ART:
    {
        deleteArticle(ms);
        break;
    }
    case Protocol::COM_GET_ART:
    {
        getArticle(ms);
        break;
    }
    default:
        cout << endl;
        cout << ">>Invalid command (" << command << ")!" << endl;
        return;
    }
    MessageHandler::send(conn, ms);
    try
    {
        ms = MessageHandler::recieve(conn);

        // handle the response
        switch (ms.getCommand())
        {
        case Protocol::ANS_LIST_NG:
        {
            ansListGroups(ms);
            break;
        }
        case Protocol::ANS_LIST_ART:
        {
            ansListArticles(ms);
            break;
        }
        case Protocol::ANS_CREATE_NG:
        {
            ansCreateGroup(ms);
            break;
        }
        case Protocol::ANS_CREATE_ART:
        {
            ansCreateArticle(ms);
            break;
        }
        case Protocol::ANS_DELETE_NG:
        {
            ansDeleteGroup(ms);
            break;
        }
        case Protocol::ANS_DELETE_ART:
        {
            ansDeleteArticle(ms);
            break;
        }
        case Protocol::ANS_GET_ART:
        {
            ansGetArticle(ms);
            break;
        }
        default:
            break;
        }
    }
    catch (MessageException &)
    {
        cout << ">>Recieved ILLEGAL MESSAGE!" << endl;
        cout << endl;
    }
}

int app(shared_ptr<Connection> conn)
{
    // print the commands here
    int command;
    print_commands();
    cout << "Type a command (number): ";
    while (true)
    {
        cin >> command;
        try
        {
            handleCommand(conn, command);
            cout << endl;
        }
        catch (ConnectionClosedException &)
        {
            cout << " no reply from server. Exiting." << endl;
            return 1;
        }
        print_commands();
        cout << "Next command (number): ";
    }
    return 0;
}

int main(int argc, char *argv[])
{
    return app(make_shared<Connection>(init(argc, argv)));
}
