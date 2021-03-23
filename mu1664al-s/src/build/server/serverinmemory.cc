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
        SharedConnection conn = server.waitForActivity();
        if (conn != nullptr)
        {
            try
            {
                shared_ptr<DBInterface> db = make_shared<DBInMemory>(DBInMemory());
                MessageHandler msh = MessageHandler(db, conn);
                msh.handle();
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
