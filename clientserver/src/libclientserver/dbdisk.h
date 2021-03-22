#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
};

#endif
