#include "dbinmemory.h"
#include "serverbase.h"
#include <stdexcept>

using std::cout;
using std::endl;

Server init(int argc, char *argv[])
{
    if (argc < 2 || argc > 2)
    {
        cerr << "Usage: serverinmemory port-number" << endl;
        exit(1);
    }

    int port = -1;
    try
    {
        port = stoi(argv[1]);
    }
    catch (exception &e)
    {
        cerr << "Wrong format for port number. " << e.what() << endl;
        exit(2);
    }

    Server server(port);
    if (!server.isReady())
    {
        cerr << "Server initialization error." << endl;
        exit(3);
    }
    return server;
}

int main(int argc, char *argv[])
{
    ServerBase base_server = ServerBase();
    Server server = init(argc, argv);
    shared_ptr<DBInterface> db = make_shared<DBInMemory>();
    base_server.run(server, db);
    return 0;
}
