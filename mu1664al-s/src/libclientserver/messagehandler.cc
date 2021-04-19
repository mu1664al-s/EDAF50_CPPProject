#include "messagehandler.h"
#include <iostream>
#include <algorithm>

static int decNum(const string &str)
{
    unsigned char byte1 = str[0];
    unsigned char byte2 = str[1];
    unsigned char byte3 = str[2];
    unsigned char byte4 = str[3];
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

static const string read(shared_ptr<Connection> conn, int N)
{
    string buffer;
    for (int i = 0; i < N; i++)
    {
        buffer += conn->read();
    }
    return buffer;
}

static void readNumb(shared_ptr<Connection> conn, Message &ms)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    if (proto != Protocol::PAR_NUM)
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not a number parameter"};
    }
    else
    {
        int N = decNum(read(conn, 4));
        ms.addNumParam(N);
    }
}

static void readStr(shared_ptr<Connection> conn, Message &ms)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    if (proto != Protocol::PAR_STRING)
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not a number parameter"};
    }
    else
    {
        int N = decNum(read(conn, 4));
        ms.addStrParam(read(conn, N));
    }
}

static void readList(shared_ptr<Connection> conn, Message &ms)
{
    readNumb(conn, ms);
    int N = ms.getParmaters().back().N;
    for (int i = 0; i < N; i++)
    {
        readNumb(conn, ms);
        readStr(conn, ms);
    }
}

static void readCOMEnd(shared_ptr<Connection> conn)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    if (proto != Protocol::COM_END)
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not a COM_END byte"};
    }
}

static void readANSEnd(shared_ptr<Connection> conn)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    if (proto != Protocol::ANS_END)
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not an ANS_END byte"};
    }
}

static void readStatus(shared_ptr<Connection> conn, Message &ms)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    if (proto == Protocol::ANS_ACK || proto == Protocol::ANS_NAK)
    {
        ms.setStatus(proto);
    }
}

static void readError(shared_ptr<Connection> conn, Message &ms, const vector<Protocol> &valid_err)
{
    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    auto it = find_if(valid_err.begin(), valid_err.end(), [&proto](const Protocol &type) { return type == proto; });
    if (it == valid_err.end())
    {
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not an error byte"};
    }
    ms.setError(proto);
}

static void writeString(shared_ptr<Connection> conn, const string &str)
{
    for (unsigned char c : str)
    {
        conn->write(c);
    }
}

Message MessageHandler::recieve(shared_ptr<Connection> conn)
{
    Message ms = Message();
    string _N = "";
    string str = "";

    unsigned char ch = conn->read();
    Protocol proto = static_cast<Protocol>(ch);
    // read command
    ms.setCommand(proto);

    // read command bytes and validate
    switch (ms.getCommand())
    {
    case Protocol::COM_LIST_NG:
        readCOMEnd(conn);
        break;
    case Protocol::COM_CREATE_NG:
        readStr(conn, ms);
        readCOMEnd(conn);
        break;
    case Protocol::COM_DELETE_NG:
        readNumb(conn, ms);
        readCOMEnd(conn);
        break;
    case Protocol::COM_LIST_ART:
        readNumb(conn, ms);
        readCOMEnd(conn);
        break;
    case Protocol::COM_CREATE_ART:
        readNumb(conn, ms);
        readStr(conn, ms);
        readStr(conn, ms);
        readStr(conn, ms);
        readCOMEnd(conn);
        break;
    case Protocol::COM_DELETE_ART:
        readNumb(conn, ms);
        readNumb(conn, ms);
        readCOMEnd(conn);
        break;
    case Protocol::COM_GET_ART:
        readNumb(conn, ms);
        readNumb(conn, ms);
        readCOMEnd(conn);
        break;
    //answers
    case Protocol::ANS_LIST_NG:
        readList(conn, ms);
        readANSEnd(conn);
        break;
    case Protocol::ANS_CREATE_NG:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_ALREADY_EXISTS});
        }
        readANSEnd(conn);
        break;
    case Protocol::ANS_DELETE_NG:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_DOES_NOT_EXIST});
        }
        readANSEnd(conn);
        break;
    case Protocol::ANS_LIST_ART:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_ACK)
        {
            readList(conn, ms);
        }
        else if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_DOES_NOT_EXIST});
        }
        readANSEnd(conn);
        break;
    case Protocol::ANS_CREATE_ART:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_DOES_NOT_EXIST});
        }
        readANSEnd(conn);
        break;
    case Protocol::ANS_DELETE_ART:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_DOES_NOT_EXIST, Protocol::ERR_ART_DOES_NOT_EXIST});
        }
        readANSEnd(conn);
        break;
    case Protocol::ANS_GET_ART:
        readStatus(conn, ms);
        if (ms.getStatus() == Protocol::ANS_ACK)
        {
            readStr(conn, ms);
            readStr(conn, ms);
            readStr(conn, ms);
        }
        else if (ms.getStatus() == Protocol::ANS_NAK)
        {
            readError(conn, ms, {Protocol::ERR_NG_DOES_NOT_EXIST, Protocol::ERR_ART_DOES_NOT_EXIST});
        }
        readANSEnd(conn);
        break;

    default:
        throw MessageException{MessageExceptionType::ILLEGAL_MESSAGE, "not a command or answer byte"};
        break;
    }
    return ms;
}

void MessageHandler::send(shared_ptr<Connection> conn, const Message &message)
{
    writeString(conn, message.encode());
}