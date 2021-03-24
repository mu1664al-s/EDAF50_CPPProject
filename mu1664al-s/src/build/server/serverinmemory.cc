#include "dbinmemory.h"
#include "serverbase.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    ServerBase base_server = ServerBase();
    Server server = base_server.init(argc, argv);

    while (true)
    {
        shared_ptr<Connection> conn = server.waitForActivity();
        if (conn != nullptr)
        {
            try
            {
                const DBInterface &db = DBInMemory();
                MessageHandler msh = MessageHandler(conn);
                Message ms = msh.recieve();
                base_server.exec(msh, db, ms);
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
