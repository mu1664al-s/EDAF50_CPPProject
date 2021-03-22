
#ifndef MESSAGES_HANDLER_H
#define MESSAGES_HANDLER_H

#include <string>

struct Message
{
};

enum class MessageExceptionType
{
    ILLEGAL_MESSAGE = 0, // connection will disconnect when thrown
    GROUP_NOT_FOUND = 1,
    ARTICLE_NOT_FOUND = 2,
};

struct MessageException
{
    int type;
    std::string message;
};

class MessageHandler
{
public:
    ~MessageHandler();
    MessageHandler() = default;

    // parse and handle messages. throws exception
    // Server requests are rejected if there are no db provided
    // Response message is generated and returned
    Message handle(const char *package) const;
    const char *package(const Message &message) const;

private:
    Message decode(const char *package) const;
    const char *encode(const Message &message) const;
};

#endif
