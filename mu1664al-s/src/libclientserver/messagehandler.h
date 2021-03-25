
#ifndef MESSAGES_HANDLER_H
#define MESSAGES_HANDLER_H

#include "protocol.h"
#include "connection.h"
#include <utility>
#include <memory>
#include <vector>
#include <string>

using std::make_pair;
using std::shared_ptr;
using std::string;

struct Param
{
    Protocol type;
    int N;
    string str;
};

enum class MessageExceptionType
{
    ILLEGAL_MESSAGE = 0,
};

struct MessageException
{
    MessageExceptionType type;
    string message;
};

using Parameter = std::pair<Param, Param>;
using Parameters = std::vector<Parameter>;

struct Message
{
    Protocol command;
    Protocol status;
    Parameters parameters; //
    Protocol end;
};

class MessageHandler
{
public:
    ~MessageHandler(){};
    MessageHandler(const shared_ptr<Connection> &conn) : conn(conn) {}

    // parse and handle messages. throws exception
    // Server requests are rejected iConnectionf there are no db provided
    // Response message is generated and returned
    Message recieve() const;
    void sendRequest(Protocol command, const Parameters &parameters) const;
    Parameter numParam(int num) const;
    Parameter strParam(string str) const;
    void send(const Message &message) const;

private:
    const shared_ptr<Connection> &conn;
    Message decode(string package) const;
    string encode(const Message &message) const;
    void exec(const Message &message) const;
    string readPackage() const;
    int decodeNumber(const string &str) const;
    string encodeNumber(int num) const;
    void writeString(const string &s) const;
};

#endif
