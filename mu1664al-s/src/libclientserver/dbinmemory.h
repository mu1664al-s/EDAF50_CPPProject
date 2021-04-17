#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H
#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    virtual void writeArticle(size_t group, const Article &article); // done

    virtual const Article readArticle(size_t group, size_t article); // done

    virtual const std::shared_ptr<vector<Group>> readGroups(); // done

    virtual void writeGroup(const string &name); // done

    virtual const std::shared_ptr<vector<Article>> readArticles(size_t group); // done

    virtual void deleteArticle(size_t group, size_t article); // done

    virtual void deleteGroup(size_t group); // done

private:
    vector<Group> groups;
    vector<Group>::iterator checkGroups(size_t group);
    vector<Article>::iterator checkArticles(vector<Group>::iterator &it, size_t article);
};

#endif
