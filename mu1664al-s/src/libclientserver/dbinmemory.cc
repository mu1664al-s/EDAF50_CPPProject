#include "dbinmemory.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
using namespace std;

DBInMemory::~DBInMemory()
{
}

template <typename T>
size_t nextId(const vector<T> &list)
{
    if (list.size() > 0)
    {
        return list.back().id + 1;
    }
    return 1;
}

template <typename T>
void checkList(const vector<T> &list, const string &title, const DBException &e)
{
    auto it = find_if(list.begin(), list.end(), [&title](const T &item) { return item.title == title; });
    if (it != list.end())
    {
        throw e;
    }
}

void DBInMemory::writeArticle(size_t group, const Article &article)
{
    auto it = checkGroups(group);
    checkList<Article>(it->articles, article.title, DBException{DBExceptionType::ARTICLE_ALREADY_EXISTS, " article exists"});

    size_t newid = nextId<Article>(it->articles);
    Article newarticle{newid, article.title, article.author, article.text};
    it->articles.push_back(newarticle);
}

const Article DBInMemory::readArticle(size_t group, size_t article)
{
    auto it = checkGroups(group);
    return *checkArticles(it, article);
}

const std::shared_ptr<vector<Group>> DBInMemory::readGroups()
{
    return std::make_shared<vector<Group>>(this->groups);
}

void DBInMemory::writeGroup(const string &title)
{
    checkList<Group>(groups, title, DBException{DBExceptionType::GROUP_ALREADY_EXISTS, " group exists"});
    size_t newid = nextId<Group>(groups);
    Group newgroup{newid, title, vector<Article>{}};
    this->groups.push_back(newgroup);
}

const std::shared_ptr<vector<Article>> DBInMemory::readArticles(size_t group)
{
    auto it = checkGroups(group);
    return std::make_shared<vector<Article>>(it->articles);
}

void DBInMemory::deleteArticle(size_t group, size_t article)
{
    auto it = checkGroups(group);
    auto itart = checkArticles(it, article);
    it->articles.erase(itart);
}

void DBInMemory::deleteGroup(size_t group)
{
    auto it = checkGroups(group);
    groups.erase(it);
}

vector<Group>::iterator DBInMemory::checkGroups(size_t group)
{
    // binary search
    auto it = lower_bound(groups.begin(), groups.end(), group, [](const Group &g, size_t id) { return g.id < id; });
    if (it == groups.end() || it->id != group)
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, ""};
    }
    return it;
}

vector<Article>::iterator DBInMemory::checkArticles(vector<Group>::iterator &it, size_t article)
{
    // binary search
    auto itar = lower_bound(it->articles.begin(), it->articles.end(), article, [](const Article &a, size_t id) { return a.id < id; });
    if (itar == it->articles.end() || itar->id != article)
    {
        throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""};
    }
    return itar;
}