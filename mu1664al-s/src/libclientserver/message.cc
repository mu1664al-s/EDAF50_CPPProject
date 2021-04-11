#include "message.h"
#include <regex>
#include <iostream>

Message::Message(const string &package)
{
    parameters = vector<Parameter>{};
    int param_offset = 1;
    command = static_cast<Protocol>(package[0]);
    //std::cout << "command: " << static_cast<int>(package[0]) << std::endl;
    if (command > Protocol::ANS_LIST_NG)
    {
        //std::cout << "checking answer" << std::endl;
        if (package.length() <= 2)
        {
            throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "Message too short!"};
        }
        status = static_cast<Protocol>(package[1]);
        param_offset = 2;
        if (status == Protocol::ANS_NAK)
        {
            if (package.length() > 4)
            {
                throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "Message too long!"};
            }
            error = static_cast<Protocol>(package[2]);
            param_offset = 3;
        }
    }
    // add the parameters
    for (; param_offset < package.length() - 1;)
    {
        Protocol type = static_cast<Protocol>(package[param_offset]);
        int N = decNum(package.substr(param_offset + 1, 4));
        //std::cout << "type: " << static_cast<int>(package[param_offset]) << std::endl;
        //std::cout << "N: " << N << std::endl;
        param_offset += 5;
        string str = "";
        if (type == Protocol::PAR_STRING)
        {
            str = package.substr(param_offset, N);
            //std::cout << "string param: " << str << std::endl;
            param_offset += N;
        }
        parameters.emplace_back(Parameter{type, N, str});
    }
}

string Message::encodeParams() const
{
    string s = "";
    int c = 0;
    for (const Parameter &param : parameters)
    {
        s += static_cast<unsigned char>(param.type);
        s += encNum(param.N);
        s += param.str;
        //std::cout << "parameters " << parameters.size() << ": " << param.N << std::endl;
    }
    return s;
}

const string Message::encode() const
{
    string s = "";
    s += static_cast<unsigned char>(command);
    //std::cout << "command set" << std::endl;
    if (command <= Protocol::ANS_LIST_NG) // response without status
    {
        s += encodeParams();
        //std::cout << "parameters set" << std::endl;
    }
    else
    {
        // handling response with status and error
        s += static_cast<unsigned char>(status);
        //std::cout << "status set" << std::endl;
        if (status == Protocol::ANS_NAK)
        {
            s += static_cast<unsigned char>(error);
            //std::cout << "error set" << std::endl;
        }
        else
        {
            s += encodeParams();
            //std::cout << "parameters set" << std::endl;
        }
    }

    if (command > Protocol::COM_END)
    {
        s += static_cast<unsigned char>(Protocol::ANS_END);
    }
    else
    {
        s += static_cast<unsigned char>(Protocol::COM_END);
    }
    //std::cout << "end set" << std::endl;
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