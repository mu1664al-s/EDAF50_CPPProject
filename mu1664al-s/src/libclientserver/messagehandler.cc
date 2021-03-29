#include "messagehandler.h"

Message MessageHandler::recieve() const
{
    Message ms = Message(readPackage()); // decode
    if (ms.getStatus() == Protocol::ANS_ACK || ms.getStatus() == Protocol::ANS_NAK)
    {
        return ms;
    }
    return Message{};
}
void MessageHandler::send(const Message &message) const
{
    writeString(message.encode());
}

string MessageHandler::readPackage() const
{
    string s;
    unsigned char ch;
    while ((ch = conn->read()) != '$')
    {
        s += ch;
    }
    return s;
}

void MessageHandler::writeString(const string &str) const
{
    for (char c : str)
    {
        conn->write(c);
    }
    conn->write('$');
}