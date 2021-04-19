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
    void run(Server &server, shared_ptr<DBInterface> db)
    {
        while (true)
        {
            shared_ptr<Connection> conn = server.waitForActivity();
            if (conn != nullptr)
            {
                try
                {
                    exec(conn, db);
                }
                catch (ConnectionClosedException &)
                {
                    server.deregisterConnection(conn);
                    cout << "Client closed connection" << endl;
                }
                catch (MessageException &)
                {
                    server.deregisterConnection(conn);
                    cout << "Client disconnected for illegal message" << endl;
                }
            }
            else
            {
                conn = make_shared<Connection>();
                server.registerConnection(conn);
                cout << "New client connects" << endl;
            }
        }
    };
    void exec(shared_ptr<Connection> conn,
              shared_ptr<DBInterface> db)
    {
        Message ms = Message();
        const Message &message = MessageHandler::recieve(conn);
        switch (message.getCommand())
        {
        case Protocol::COM_LIST_NG:
        {
            listGroups(db, ms);
            break;
        }
        case Protocol::COM_LIST_ART:
        {
            listArticles(db, message, ms);
            break;
        }
        case Protocol::COM_CREATE_NG:
        {
            createGroup(db, message, ms);
            break;
        }
        case Protocol::COM_CREATE_ART:
        {
            createArticle(db, message, ms);
            break;
        }
        case Protocol::COM_DELETE_NG:
        {
            deleteGroup(db, message, ms);
            break;
        }
        case Protocol::COM_DELETE_ART:
        {
            deleteArticle(db, message, ms);
            break;
        }
        case Protocol::COM_GET_ART:
        {
            getArticle(db, message, ms);
            break;
        }
        default:
            break;
        }
        MessageHandler::send(conn, ms);
    };

private:
    void listGroups(shared_ptr<DBInterface> database, Message &ms)
    {
        const std::shared_ptr<vector<Group>> groups = database->readGroups();
        ms.setCommand(Protocol::ANS_LIST_NG).addNumParam(groups->size());
        for (const Group &g : *groups)
        {
            ms.addNumParam(g.id).addStrParam(g.title);
        }
    }

    void listArticles(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            const int &id = message.getParmaters()[0].N;
            const std::shared_ptr<vector<Article>> &articles = database->readArticles(id);
            ms.setCommand(Protocol::ANS_LIST_ART).setStatus(Protocol::ANS_ACK).addNumParam(articles->size());
            for (const Article &a : *articles)
            {
                ms.addNumParam(a.id).addStrParam(a.title);
            }
        }
        catch (DBException &)
        {
            ms.setCommand(Protocol::ANS_LIST_ART).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void createGroup(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            const string &name = message.getParmaters()[0].str;
            database->writeGroup(name);
            ms.setCommand(Protocol::ANS_CREATE_NG).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            ms.setCommand(Protocol::ANS_CREATE_NG).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_ALREADY_EXISTS);
        }
    }

    void createArticle(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            const vector<Parameter> &params = message.getParmaters();
            const int &id = params[0].N;
            const string &title = params[1].str;
            const string &author = params[2].str;
            const string &text = params[3].str;
            database->writeArticle(id, Article{0, title, author, text});
            ms.setCommand(Protocol::ANS_CREATE_ART).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            ms.setCommand(Protocol::ANS_CREATE_ART).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void deleteGroup(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            const int &id = message.getParmaters()[0].N;
            database->deleteGroup(id);
            ms.setCommand(Protocol::ANS_DELETE_NG).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &)
        {
            ms.setCommand(Protocol::ANS_DELETE_NG).setStatus(Protocol::ANS_NAK).setError(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
    }

    void deleteArticle(shared_ptr<DBInterface> database, const Message &message, Message &ms)
    {
        try
        {
            const vector<Parameter> &params = message.getParmaters();
            const int &group = params[0].N;
            const int &article = params[1].N;
            database->deleteArticle(group, article);
            ms.setCommand(Protocol::ANS_DELETE_ART).setStatus(Protocol::ANS_ACK);
        }
        catch (DBException &e)
        {
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
            const vector<Parameter> &params = message.getParmaters();
            const int &group_id = params[0].N;
            const int &article_id = params[1].N;
            const Article &article = database->readArticle(group_id, article_id);
            ms.setCommand(Protocol::ANS_GET_ART).setStatus(Protocol::ANS_ACK).addStrParam(article.title).addStrParam(article.author).addStrParam(article.text);
        }
        catch (DBException &e)
        {
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
};

#endif
