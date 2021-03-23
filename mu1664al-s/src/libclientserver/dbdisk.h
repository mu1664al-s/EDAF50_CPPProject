#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
    void writeArticle(ID group, Article article);

    Article readArticle(ID group, ID article);

    Groups readGroups();

    void writeGroup(string name);

    Articles readArticles(ID group);

    void deleteArticle(ID group, ID article);

    void deleteGroup(ID group);
};

#endif
