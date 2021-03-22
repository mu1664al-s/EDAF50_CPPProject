#include "dbdisk.h"
#include "serverbase.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    ServerBase base_server = ServerBase();
    Server server = base_server.init(argc, argv);

    while (true)
    {
        SharedConnection conn = server.waitForActivity();
        if (conn != nullptr)
        {
            try
            {
                int nbr = base_server.readNumber(conn);
                string result;
                if (nbr > 0)
                {
                    result = "positive";
                }
                else if (nbr == 0)
                {
                    result = "zero";
                }
                else
                {
                    result = "negative";
                }
                base_server.writeString(conn, result);
            }
            catch (ConnectionClosedException &)
            {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        }
        else
        {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
    return 0;
}
