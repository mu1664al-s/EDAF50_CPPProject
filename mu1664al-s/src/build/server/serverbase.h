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
using std::cout;
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
    void listGroups(shared_ptr<DBInterface> database, Message &ms)
    {
        vector<Group> groups = database->readGroups();
        ms.setCommand(Protocol::ANS_LIST_NG).addNumParam(groups.size());
        for (Group g : groups)
        {
            ms.addNumParam(g.id).addStrParam(g.title);
        }
    }

    void listArticles(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            int id = message.getParmaters()[0].N;
            vector<Article> articles = database->readArticles(id);
            ms.setCommand(Protocol::ANS_LIST_ART).setStatus(Protocol::ANS_ACK).addNumParam(articles.size());
            for (Article a : articles)
            {
                ms.addNumParam(a.id).addStrParam(a.title);
            }
        }
        catch (DBException &)
        {
            cout << "DBException" << endl;
            ms.setCommand(Protocol::ANS_LIST_ART).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void createGroup(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            string name = message.getParmaters()[0].str;
            database->writeGroup(name);
            ms.setCommand(Protocol::ANS_CREATE_NG).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            cout << "DBException" << endl;
            ms.setCommand(Protocol::ANS_CREATE_NG).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_ALREADY_EXISTS);
        }
    }

    void createArticle(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            vector<Parameter> params = message.getParmaters();
            int id = params[0].N;
            string title = params[1].str;
            string author = params[2].str;
            string text = params[3].str;
            database->writeArticle(id, Article{0, title, author, text});
            ms.setCommand(Protocol::ANS_CREATE_ART).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            cout << "DBException" << endl;
            ms.setCommand(Protocol::ANS_CREATE_ART).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void deleteGroup(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            int id = message.getParmaters()[0].N;
            database->deleteGroup(id);
            ms.setCommand(Protocol::ANS_DELETE_NG).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            cout << "DBException" << endl;
            ms.setCommand(Protocol::ANS_DELETE_NG).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void deleteArticle(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            vector<Parameter> params = message.getParmaters();
            int group = params[0].N;
            int article = params[1].N;
            database->deleteArticle(group, article);
            ms.setCommand(Protocol::ANS_DELETE_ART).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &e)
        {
            cout << "DBException" << endl;
            Protocol error;
            if (e.type == DBExceptionType::ARTICLE_NOT_FOUND)
            {
                error = Protocol::ERR_ART_DOES_NOT_EXIST;
            }
            else
            {
                error = Protocol::ERR_NG_DOES_NOT_EXIST;
            }
            ms.setCommand(Protocol::ANS_DELETE_ART).setStatus(Protocol::ANS_NAK).setError(error);
        }
    }

    void getArticle(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            vector<Parameter> params = message.getParmaters();
            int group_id = params[0].N;
            int article_id = params[1].N;
            Article article = database->readArticle(group_id, article_id);
            ms.setCommand(Protocol::ANS_GET_ART).setStatus(Protocol::ANS_ACK).addStrParam(article.title).addStrParam(article.author).addStrParam(article.text);
        }
        catch (DBException &e)
        {
            cout << "DBException" << endl;
            Protocol error;
            if (e.type == DBExceptionType::ARTICLE_NOT_FOUND)
            {
                error = Protocol::ERR_ART_DOES_NOT_EXIST;
            }
            else
            {
                error = Protocol::ERR_NG_DOES_NOT_EXIST;
            }
            ms.setCommand(Protocol::ANS_GET_ART).setStatus(Protocol::ANS_NAK).setError(error);
        }
    }

    void exec(shared_ptr<Connection> conn,
              shared_ptr<DBInterface> database)
    {
        try
        {
            Message ms = Message();
            const Message &message = MessageHandler::recieve(conn);
            switch (message.getCommand())
            {
            case Protocol::COM_LIST_NG:
            {
                listGroups(database, ms);
                break;
            }
            case Protocol::COM_LIST_ART:
            {
                listArticles(database, message, ms);
                break;
            }
            case Protocol::COM_CREATE_NG:
            {
                createGroup(database, message, ms);
                break;
            }
            case Protocol::COM_CREATE_ART:
            {
                createArticle(database, message, ms);
                break;
            }
            case Protocol::COM_DELETE_NG:
            {
                deleteGroup(database, message, ms);
                break;
            }
            case Protocol::COM_DELETE_ART:
            {
                deleteArticle(database, message, ms);
                break;
            }
            case Protocol::COM_GET_ART:
            {
                getArticle(database, message, ms);
                break;
            }
            default:
                break;
            }
            MessageHandler::send(conn, ms);
        }
        catch (MessageException &)
        {
            cout << "ILLEGAL MESSAGE" << endl;
        }
    };

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
