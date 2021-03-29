#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H

#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    DBInMemory() = default;
    virtual void writeArticle(int group, const Article &article);

    virtual const Article readArticle(int group, int article);

    virtual const vector<Group> readGroups();

    virtual void writeGroup(const string &name);

    virtual const vector<Article> readArticles(int group);

    virtual void deleteArticle(int group, int article);

    virtual void deleteGroup(int group);
};

#endif
