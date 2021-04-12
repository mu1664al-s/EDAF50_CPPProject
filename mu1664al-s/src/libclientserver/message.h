#include <utility>
#include <vector>
#include <string>
#include "protocol.h"

using std::make_pair;
using std::string;
using std::vector;

struct Parameter
{
    const Protocol type;
    const int N;
    const string str;
};

enum class MessageExceptionType
{
    ILLEGAL_MESSAGE = 0,
};

struct MessageException
{
    const MessageExceptionType type;
    const string message;
};

class Message
{
public:
    Message() = default;
    Message(const string &package); // parse and validate message package
    const string encode() const;
    Message &addNumParam(int num);
    Message &addStrParam(const string &str);
    Message &setCommand(const Protocol &command)
    {
        this->command = command;
        return *this;
    };
    Message &setStatus(const Protocol &status)
    {
        this->status = status;
        return *this;
    };
    Message &setError(const Protocol &error)
    {
        this->error = error;
        return *this;
    };
    const Protocol &getCommand() const { return command; };
    const Protocol &getStatus() const { return status; };
    const vector<Parameter> &getParmaters() const { return parameters; };
    const Protocol &getError() const { return error; };

private:
    Protocol command;
    Protocol status;
    Protocol error;
    vector<Parameter> parameters;

    int decNum(const string &str) const;
    const string encNum(int num) const;
    string encodeParams() const;
};