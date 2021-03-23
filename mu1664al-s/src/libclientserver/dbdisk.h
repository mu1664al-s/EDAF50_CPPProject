#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
    bool writeArticle(ID group, Article article);

    Article readArticle(ID group, ID article);

    Groups readGroups();

    void writeGroup(string name);

    Articles readArticles(ID group);

    bool deleteArticle(ID group, ID article);

    bool deleteGroup(ID group);
};

#endif
