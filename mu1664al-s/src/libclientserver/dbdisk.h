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
    virtual void writeArticle(int group, const Article &article);

    virtual const Article readArticle(int group, int article);

    virtual const vector<Group> &readGroups();

    virtual void writeGroup(const string &title);

    virtual const vector<Article> readArticles(int group);

    virtual void deleteArticle(int group, int article);

    virtual void deleteGroup(int group);
    void getids();
    void getnames();

private:
    string fs_root;
    vector<Group> groups;
    vector<Group>::iterator checkRegister(int group);
    pair<int, string> readPair(const string &line);
};

#endif