
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
    Protocol status;
    std::vector<Parameter> parameters;
    Protocol end;
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
    void handle(const char *package) const;
    void send(const Message &message) const;

private:
    const shared_ptr<Connection> &conn;
    const shared_ptr<DBInterface> &db;
    Message decode(const char *package) const;
    string encode(const Message &message) const;
    void exec(const Message &message) const;
    unsigned int readNumber() const;
    void writeString(const string &s) const;
};

#endif
