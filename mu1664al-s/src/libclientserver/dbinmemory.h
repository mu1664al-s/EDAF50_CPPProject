#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H

#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    DBInMemory() = default;
    bool writeArticle(ID group, Article article) = 0;

    Article readArticle(ID group, ID article) = 0;

    Groups readGroups() = 0;

    void writeGroup(string name) = 0;

    Articles readArticles(ID group) = 0;

    bool deleteArticle(ID group, ID article) = 0;

    bool deleteGroup(ID group) = 0;
};

#endif
