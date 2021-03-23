#include "messagehandler.h"

Message MessageHandler::handle(const char *package) const
{
    Message ms = decode(package); // decode
    if (ms.response == Protocol::ANS_ACK)
        return ms;
    return exec(ms);
}
void MessageHandler::send(const Message &message) const
{
}

Message MessageHandler::decode(const char *package) const
{
}

Message MessageHandler::exec(const Message &message) const
{
    switch (message.command)
    {
    case Protocol::COM_LIST_NG:
        // read rest of the parameters and process
        // generate respose
        return Message{};

    default:
        break;
    }
}