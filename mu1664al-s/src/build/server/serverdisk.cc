#include "dbdisk.h"
#include "serverbase.h"
#include <string>

using std::cout;
using std::endl;
using std::string;

string fs_root;

Server init(int argc, char *argv[])
{
    if (argc < 3 || argc > 3)
    {
        cerr << "Usage: serverdisk filesystem-root-path/ port-number" << endl;
        exit(1);
    }

    int port = -1;
    try
    {
        port = stoi(argv[2]);
    }
    catch (exception &e)
    {
        cerr << "Wrong format for port number. " << e.what() << endl;
        exit(2);
    }

    fs_root = argv[1];
    if (fs_root.back() != '/')
    {
        fs_root += '/';
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
    shared_ptr<DBInterface> db = make_shared<DBDisk>(fs_root);
    base_server.run(server, db);
    return 0;
}
