#include "messagehandler.h"

Message MessageHandler::recieve() const
{
    Message ms = decode(readPackage()); // decode
    if (ms.status == Protocol::ANS_ACK || ms.status == Protocol::ANS_NAK)
    {
        return ms;
    }
    return Message{};
}
void MessageHandler::send(const Message &message) const
{
    writeString(encode(message));
}

Message MessageHandler::decode(const string &package) const
{
    // generate a message from byte data
    Message ms = Message{};
    return ms;
}

string MessageHandler::encode(const Message &message) const
{
    return "";
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

void MessageHandler::sendRequest(const Protocol &command, const Parameters &parameters) const
{
}

Parameter MessageHandler::numParam(int num) const
{
    return make_pair(Param{Protocol::PAR_NUM, num, ""}, Param{});
}

Parameter MessageHandler::strParam(const string &str) const
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

void MessageHandler::writeString(const string &str) const
{
    for (char c : str)
    {
        conn->write(c);
    }
    conn->write('$');
}