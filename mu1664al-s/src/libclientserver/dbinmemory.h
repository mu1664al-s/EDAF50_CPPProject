#ifndef DB_IN_MEMORY_H
#define DB_IN_MEMORY_H
#include "dbinterface.h"

class DBInMemory : public DBInterface
{
public:
    ~DBInMemory();
    virtual void writeArticle(int group, const Article &article); // done

    virtual const Article readArticle(int group, int article); // done

    virtual const std::shared_ptr<vector<Group>> readGroups(); // done

    virtual void writeGroup(const string &name); // done

    virtual const std::shared_ptr<vector<Article>> readArticles(int group); // done

    virtual void deleteArticle(int group, int article); // done

    virtual void deleteGroup(int group); // done

private:
    vector<Group> groups;
    vector<Group>::iterator checkGroups(int group);
    vector<Article>::iterator checkArticles(vector<Group>::iterator &it, int article);
};

#endif
