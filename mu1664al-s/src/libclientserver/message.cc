#include "message.h"
#include <iostream>

const string encNum(int value)
{
    string str = "";
    str += (value >> 24) & 0xFF;
    str += (value >> 16) & 0xFF;
    str += (value >> 8) & 0xFF;
    str += value & 0xFF;
    return str;
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