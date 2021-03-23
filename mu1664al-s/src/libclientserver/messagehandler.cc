#include "messagehandler.h"
#include <iostream>

using std::cout;
using std::endl;

void MessageHandler::handle(const char *package) const
{
    Message ms = decode(package); // decode
    if (ms.status == Protocol::ANS_ACK)
    {
        // handle response package
        // print result
        return;
    }
    if (ms.status == Protocol::ANS_NAK)
    {
        // handle response package
        // print result
        return;
    }
    exec(ms);
}
void MessageHandler::send(const Message &message) const
{
    writeString(encode(message));
}

Message MessageHandler::decode(const char *package) const
{
}

string MessageHandler::encode(const Message &message) const
{
}

unsigned int MessageHandler::readNumber() const
{
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MessageHandler::writeString(const string &s) const
{
    for (char c : s)
    {
        conn->write(c);
    }
    conn->write('$');
}

void MessageHandler::exec(const Message &message) const
{
    Message ms = Message{};
    try
    {
        switch (message.command)
        {
        case Protocol::COM_LIST_NG:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_LIST_ART:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_CREATE_NG:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_CREATE_ART:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_DELETE_NG:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_DELETE_ART:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        case Protocol::COM_GET_ART:
        {
            // read rest of the parameters and process
            // generate respose
            break;
        }
        default:
            break;
        }
    }
    catch (DBException &err)
    {
        // generate exception message
    }
    send(ms);
}