#include "messagehandler.h"
#include <iostream>

using std::cout;
using std::endl;

void MessageHandler::handle() const
{
    Message ms = decode(readPackage()); // decode
    if (ms.status == Protocol::ANS_ACK)
    {
        // handle response package, can be an error
        // print result
        return;
    }
    if (ms.status == Protocol::ANS_NAK)
    {
        // handle response package, can be an error
        // print result
        return;
    }
    exec(ms);
}
void MessageHandler::send(const Message &message) const
{
    writeString(encode(message));
}

Message MessageHandler::decode(string package) const
{
    // generate a message from byte data
}

string MessageHandler::encode(const Message &message) const
{
}

string MessageHandler::readPackage() const
{
    string s;
    char ch;
    while ((ch = conn.read()) != '$')
    {
        s += ch;
    }
    return s;
}

void MessageHandler::sendRequest(Protocol command, const std::vector<Parameter> &parameters) const
{
}

Parameter MessageHandler::numParam(int num) const
{
    return make_pair(Param{Protocol::PAR_NUM, num, ""}, Param{});
}

Parameter MessageHandler::strParam(string str) const
{
    return make_pair(Param{Protocol::PAR_STRING, 0, str}, Param{});
}

int MessageHandler::decodeNumber(const string &str) const
{
    unsigned char byte1 = str[0];
    unsigned char byte2 = str[1];
    unsigned char byte3 = str[2];
    unsigned char byte4 = str[3];
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

string MessageHandler::encodeNumber(int value) const
{
    string str = "";
    str += (value >> 24) & 0xFF;
    str += (value >> 16) & 0xFF;
    str += (value >> 8) & 0xFF;
    str += value & 0xFF;
    return str;
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