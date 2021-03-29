#include "message.h"

Message::Message(const string &package)
{
    // decode and generate a message
}

const string Message::encode() const
{
    // encode and generate message string
}

int Message::decNum(const string &str) const
{
    unsigned char byte1 = str[0];
    unsigned char byte2 = str[1];
    unsigned char byte3 = str[2];
    unsigned char byte4 = str[3];
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

const string Message::encNum(int value) const
{
    string str = "";
    str += (value >> 24) & 0xFF;
    str += (value >> 16) & 0xFF;
    str += (value >> 8) & 0xFF;
    str += value & 0xFF;
    return str;
}

void Message::addNumParam(int num)
{
    parameters.push_back(Parameter{Protocol::PAR_NUM, num, ""});
}

void Message::addStrParam(const string &str)
{
    parameters.push_back(Parameter{Protocol::PAR_STRING, 0, str});
}