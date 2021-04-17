#ifndef DB_DISK_H
#define DB_DISK_H
#include <string>
#include <vector>
#include <fstream>
#include "dbinterface.h"
#include <utility>

using std::make_pair;
using std::pair;

class DBDisk : public DBInterface
{
public:
    ~DBDisk();
    DBDisk(const string &fs_root);
    virtual void writeArticle(size_t group, const Article &article);

    virtual const Article readArticle(size_t group, size_t article);

    virtual const std::shared_ptr<vector<Group>> readGroups();

    virtual void writeGroup(const string &title);

    virtual const std::shared_ptr<vector<Article>> readArticles(size_t group);

    virtual void deleteArticle(size_t group, size_t article);

    virtual void deleteGroup(size_t group);
    void getids();
    void getnames();

private:
    string fs_root;
};

#endif