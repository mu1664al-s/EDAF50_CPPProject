
#ifndef MESSAGES_HANDLER_H
#define MESSAGES_HANDLER_H

#include "dbinterface.h"
#include "protocol.h"
#include "connection.h"
#include <memory>
#include <string>
#include <vector>
#include <utility>

using std::make_pair;
using std::shared_ptr;

struct Param
{
    Protocol type;
    int N;
    string str;
};

using Parameter = std::pair<Param, Param>;
using Parameters = std::vector<Parameter>;
using SharedConnection = std::shared_ptr<Connection>;

struct Message
{
    Protocol command;
    Protocol status;
    std::vector<Parameter> parameters; //
    Protocol end;
};

class MessageHandler
{
public:
    ~MessageHandler(){};
    MessageHandler(const shared_ptr<Connection> &conn) : db(nullptr), conn(conn) {}
    MessageHandler(const shared_ptr<DBInterface> &db, const SharedConnection &conn) : db(db), conn(conn) {}

    // parse and handle messages. throws exception
    // Server requests are rejected iConnectionf there are no db provided
    // Response message is generated and returned
    void handle() const;
    void sendRequest(Protocol command, const Parameters &parameters) const;
    Parameter numParam(int num) const;
    Parameter strParam(string str) const;

private:
    const shared_ptr<DBInterface> &db;
    const SharedConnection &conn;
    Message decode(string package) const;
    string encode(const Message &message) const;
    void exec(const Message &message) const;
    string readPackage() const;
    int decodeNumber(const string &str) const;
    string encodeNumber(int num) const;
    void writeString(const string &s) const;
    void send(const Message &message) const;
};

#endif
