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
    const size_t N;
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
    Message &addNumParam(size_t num);
    Message &addStrParam(const string &str);
    Message &setCommand(const Protocol &command)
    {
        this->command = command;
        end = Protocol::ANS_END;
        if (command < Protocol::COM_END)
        {
            end = Protocol::COM_END;
        }
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
    Protocol command = Protocol::UNDEFINED;
    Protocol status = Protocol::UNDEFINED;
    Protocol error = Protocol::UNDEFINED;
    Protocol end = Protocol::UNDEFINED;
    vector<Parameter> parameters{};

    int decNum(const string &str) const;
    const string encNum(size_t num) const;
    string encodeParams() const;
};