#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H

#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    DBInMemory() = default;
    virtual void writeArticle(ID group, Article article);

    virtual Article readArticle(ID group, ID article);

    virtual Groups readGroups();

    virtual void writeGroup(string name);

    virtual Articles readArticles(ID group);

    virtual void deleteArticle(ID group, ID article);

    virtual void deleteGroup(ID group);
};

#endif
