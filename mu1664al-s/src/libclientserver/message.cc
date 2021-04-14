#include "message.h"
#include <iostream>

Message::Message(const string &package)
{
    // decode and generate a message
    command = static_cast<Protocol>(package[0]);
    size_t param_offset = 1;
    if (command > Protocol::ANS_LIST_NG)
    {
        // handle status
        status = static_cast<Protocol>(package[param_offset]);
        param_offset++;
        if (status == Protocol::ANS_NAK)
        {
            error = static_cast<Protocol>(param_offset);
            param_offset++;
        }
    }
    // add the parameters
    for (; param_offset < package.length() - 1;)
    {
        Protocol type = static_cast<Protocol>(package[param_offset]);
        int N = decNum(package.substr(param_offset + 1, 4));
        param_offset += 5;
        string str = "";
        if (type == Protocol::PAR_STRING)
        {
            str = package.substr(param_offset, N);
            param_offset += N;
        }
        parameters.emplace_back(Parameter{type, N, str});
    }
}

string Message::encodeParams() const
{
    string s = "";
    for (const Parameter &param : parameters)
    {
        s += static_cast<unsigned char>(param.type);
        s += encNum(param.N);
        s += param.str;
    }
    return s;
}

const string Message::encode() const
{
    string s = "";
    s += static_cast<unsigned char>(command);
    // handle status
    if (status != Protocol::UNDEFINED)
    {
        s += static_cast<unsigned char>(status);
        if (status == Protocol::ANS_NAK)
        {
            s += static_cast<unsigned char>(error);
        }
    }
    // add parameters
    s += encodeParams();
    // add the end byte
    s += static_cast<unsigned char>(end);
    return s;
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

Message &Message::addNumParam(int num)
{
    parameters.push_back(Parameter{Protocol::PAR_NUM, num, ""});
    return *this;
}

Message &Message::addStrParam(const string &str)
{
    parameters.push_back(Parameter{Protocol::PAR_STRING, static_cast<int>(str.length()), str});
    return *this;
}