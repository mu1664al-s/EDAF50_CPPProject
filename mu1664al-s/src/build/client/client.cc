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
            for (size_t i = 1; i < params.size();)
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
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

int numberInputHandler(const string &message)
{
    int id;
    cout << message;
    cin >> id;
    while (id == 0)
    {
        cout << endl;
        cout << ">>Error! Please input a number greater than 0." << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << endl;
        cout << message;
        cin >> id;
    }
    return id;
}

void listArticles(Message &ms)
{
    // guided process
    int id = numberInputHandler("ID of the news group: ");
    ms.setCommand(Protocol::COM_LIST_ART).addNumParam(id);
    cout << endl;
}

void ansListArticles(const Message &ms)
{
    // print response
    try
    {
        if (ms.getStatus() == Protocol::ANS_ACK)
        {
            const vector<Parameter> &params = ms.getParmaters();
            if (params[0].N > 0)
            {
                cout << ">>";
                for (size_t i = 1; i < params.size();)
                {
                    cout << params[i].N << ". ";
                    cout << params[i + 1].str << "\t";
                    i += 2;
                }
            }
            else
            {
                cout << ">>No articles found!";
            }
        }
        else if (ms.getStatus() == Protocol::ANS_NAK)
        {
            cout << ">>Error! News group alredy exists." << endl;
        }
        else
        {
            throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
        }

        cout << endl;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

void createGroup(Message &ms)
{
    cin.ignore();
    string title;
    cout << "Name the new group: ";
    getline(cin, title);
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
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

void createArticle(Message &ms)
{
    // guided process
    int id = numberInputHandler("ID of the news group: ");
    cin.ignore();
    string title;
    cout << "Title: ";
    getline(cin, title);
    string author;
    cout << "Author: ";
    getline(cin, author);
    string text;
    string text_line;
    cout << "Text: " << endl;
    //getline(cin, text, static_cast<char>(EOF));
    while (getline(cin, text_line))
    {
        if (text_line == "<<EOF>>")
        {
            break;
        }
        text += text_line + "\n";
    }
    ms.setCommand(Protocol::COM_CREATE_ART).addNumParam(id).addStrParam(title).addStrParam(author).addStrParam(text);
    cout << endl;
    return;
}

void ansCreateArticle(const Message &ms)
{
    // print response
    if (ms.getStatus() == Protocol::ANS_ACK)
    {
        cout << ">>Article Created." << endl;
    }
    else if (ms.getStatus() == Protocol::ANS_NAK)
    {
        cout << ">>Error! News group not found." << endl;
    }
    else
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

void deleteGroup(Message &ms)
{
    // guided process
    int id = numberInputHandler("ID of the news group: ");
    ms.setCommand(Protocol::COM_DELETE_NG).addNumParam(id);
    cout << endl;
}

void ansDeleteGroup(const Message &ms)
{
    // print response
    if (ms.getStatus() == Protocol::ANS_ACK)
    {
        cout << ">>News group deleted." << endl;
    }
    else if (ms.getStatus() == Protocol::ANS_NAK)
    {
        cout << ">>Error! News group not found." << endl;
    }
    else
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

void deleteArticle(Message &ms)
{
    // guided process
    int group_id = numberInputHandler("ID of the news group: ");
    int article_id = numberInputHandler("ID of the article: ");
    ms.setCommand(Protocol::COM_DELETE_ART).addNumParam(group_id).addNumParam(article_id);
    cout << endl;
}

void ansDeleteArticle(const Message &ms)
{
    // print response
    if (ms.getStatus() == Protocol::ANS_ACK)
    {
        cout << ">>Article deleted." << endl;
    }
    else if (ms.getStatus() == Protocol::ANS_NAK)
    {
        if (ms.getError() == Protocol::ERR_NG_DOES_NOT_EXIST)
        {
            cout << ">>Error! News group not found." << endl;
        }
        else if (ms.getError() == Protocol::ERR_NG_DOES_NOT_EXIST)
        {
            cout << ">>Error! Article not found." << endl;
        }
    }
    else
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
}

void getArticle(Message &ms)
{
    // guided process
    int group_id = numberInputHandler("ID of the news group: ");
    int article_id = numberInputHandler("ID of the article: ");
    ms.setCommand(Protocol::COM_GET_ART).addNumParam(group_id).addNumParam(article_id);
    cout << endl;
}

void ansGetArticle(const Message &ms)
{
    // print response
    if (ms.getStatus() == Protocol::ANS_ACK)
    {
        try
        {
            const vector<Parameter> &params = ms.getParmaters();
            string title = params[0].str;
            cout << ">>Title: " << title << endl;
            string author = params[1].str;
            cout << ">>Author: " << title << endl;
            string text = params[2].str;
            cout << ">>Text: " << endl;
            cout << text << endl;
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
            throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
        }
    }
    else if (ms.getStatus() == Protocol::ANS_NAK)
    {
        if (ms.getError() == Protocol::ERR_NG_DOES_NOT_EXIST)
        {
            cout << ">>Error! News group not found." << endl;
        }
        else if (ms.getError() == Protocol::ERR_NG_DOES_NOT_EXIST)
        {
            cout << ">>Error! Article not found." << endl;
        }
    }
    else
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, ""};
    }
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
