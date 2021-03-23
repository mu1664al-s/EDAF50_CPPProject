#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
    bool writeArticle(ID group, Article article) = 0;

    Article readArticle(ID group, ID article) = 0;

    Groups readGroups() = 0;

    void writeGroup(string name) = 0;

    Articles readArticles(ID group) = 0;

    bool deleteArticle(ID group, ID article) = 0;

    bool deleteGroup(ID group) = 0;
};

#endif
