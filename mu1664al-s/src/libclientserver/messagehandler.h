
#ifndef MESSAGES_HANDLER_H
#define MESSAGES_HANDLER_H

#include "protocol.h"
#include "connection.h"
#include "message.h"
#include <memory>

using std::shared_ptr;

class MessageHandler
{
public:
    ~MessageHandler(){};
    MessageHandler(const shared_ptr<Connection> &conn) : conn(conn) {}

    // parse and handle messages. throws exception
    // Server requests are rejected iConnectionf there are no db provided
    // Response message is generated and returned
    Message recieve() const;
    void send(const Message &message) const;

private:
    const shared_ptr<Connection> &conn;
    string readPackage() const;
    void writeString(const string &str) const;
};

#endif
