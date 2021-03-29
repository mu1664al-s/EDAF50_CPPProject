#ifndef DB_DISK_H
#define DB_DISK_H

#include "dbinterface.h"

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk() = default;
    virtual void writeArticle(int group, const Article &article);

    virtual const Article readArticle(int group, int article);

    virtual const vector<Group> readGroups();

    virtual void writeGroup(const string &name);

    virtual const vector<Article> readArticles(int group);

    virtual void deleteArticle(int group, int article);

    virtual void deleteGroup(int group);
};

#endif
