#include "messagehandler.h"
#include <iostream>

Message MessageHandler::recieve(shared_ptr<Connection> conn)
{
    string input = readPackage(conn);
    Message m = Message(input);
    /*const string output = m.encode();
    if (input == output)
    {
        std::cout << "correct encoding" << std::endl;
    }
    else
    {
        std::cout << "wrong encoding" << std::endl;
        std::cout << "expected: " << input << std::endl;
        std::cout << "got: " << output << std::endl;
    }*/
    return m;
}

void MessageHandler::send(shared_ptr<Connection> conn, const Message &message)
{
    writeString(conn, message.encode());
}

const string MessageHandler::readPackage(shared_ptr<Connection> conn)
{
    string s = "";
    unsigned char ch;
    bool readingPackage = false;
    while (true)
    {
        ch = conn->read();
        Protocol proto = static_cast<Protocol>(ch);
        if (readingPackage)
        {
            s += ch;
            // detect end and break loop
            if (proto == Protocol::COM_END || proto == Protocol::ANS_END)
            {
                break;
            }
        }

        // detect command and start buffering
        if (s == "")
        {
            if ((proto > Protocol::UNDEFINED && proto < Protocol::COM_END) || (proto >= Protocol::ANS_LIST_NG && proto < Protocol::ANS_END))
            {
                s += ch;
                readingPackage = true;
            }
            else
            {
                break;
            }
        }
    }
    return s;
}

void MessageHandler::writeString(shared_ptr<Connection> conn, const string &str)
{
    for (unsigned char c : str)
    {
        conn->write(c);
    }
}