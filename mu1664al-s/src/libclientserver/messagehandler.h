
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
    // parse and handle messages. throws exception
    // Server requests are rejected iConnectionf there are no db provided
    // Response message is generated and returned
    static Message recieve(shared_ptr<Connection> conn);
    static void send(shared_ptr<Connection> conn, const Message &message);

private:
};

#endif
