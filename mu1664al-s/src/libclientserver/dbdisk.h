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

    virtual const std::shared_ptr<vector<Group>> readGroups();

    virtual void writeGroup(const string &title);

    virtual const std::shared_ptr<vector<Article>> readArticles(int group);

    virtual void deleteArticle(int group, int article);

    virtual void deleteGroup(int group);
    void getids();
    void getnames();

private:
    string fs_root;
    pair<int, string> readPair(const string &line);
    void removeFromList(const string &path, int id);
    void appendToList(const string &path, int id, const string &title);
    std::ifstream checkFile(const string &path, const DBException &e, bool closeAfter = true);
    void readArticle(const string &path, Article &article);
    void writeArticle(const string &path, const Article &article);
    pair<int, string> titleInList(std::ifstream &in_list, const string &title, const DBException &e);

    template <typename T>
    void readList(std::ifstream &in_list, vector<T> &list)
    {
        string line;
        pair<int, string> pair;
        while (getline(in_list, line))
        {
            pair = readPair(line);
            list.push_back(T{pair.first, pair.second});
        }
        in_list.close();
    }
};

#endif