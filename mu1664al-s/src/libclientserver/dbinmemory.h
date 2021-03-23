#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H

#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    DBInMemory() = default;
    bool writeArticle(ID group, Article article);

    Article readArticle(ID group, ID article);

    Groups readGroups();

    void writeGroup(string name);

    Articles readArticles(ID group);

    bool deleteArticle(ID group, ID article);

    bool deleteGroup(ID group);
};

#endif
