
#ifndef MESSAGES_HANDLER_H
#define MESSAGES_HANDLER_H

#include "dbinterface.h"
#include "protocol.h"
#include "connection.h"
#include <string>
#include <vector>

using std::shared_ptr;

struct Parameter
{
    Protocol type;
    unsigned int N;
    string str;
};

struct Message
{
    Protocol command;
    Protocol response;
    std::vector<Parameter> parameters;
    Protocol end;
};

struct MessageException
{
    int type; // error code defined in protocol
    std::string message;
};

class MessageHandler
{
public:
    ~MessageHandler();
    MessageHandler(const shared_ptr<Connection> &conn) : db(nullptr), conn(conn) {}
    MessageHandler(const shared_ptr<DBInterface> &db, const shared_ptr<Connection> &conn) : db(db), conn(conn) {}

    // parse and handle messages. throws exception
    // Server requests are rejected iConnectionf there are no db provided
    // Response message is generated and returned
    Message handle(const char *package) const;
    void send(const Message &message) const;

private:
    const shared_ptr<Connection> &conn;
    const shared_ptr<DBInterface> &db;
    Message decode(const char *package) const;
    Message exec(const Message &message) const;
};

#endif
