#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
    virtual void writeArticle(ID group, const Article &article);

    virtual Article readArticle(ID group, ID article);

    virtual Groups readGroups();

    virtual void writeGroup(const string &name);

    virtual Articles readArticles(ID group);

    virtual void deleteArticle(ID group, ID article);

    virtual void deleteGroup(ID group);
};

#endif
