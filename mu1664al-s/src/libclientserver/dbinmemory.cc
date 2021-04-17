#include "dbinmemory.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
using namespace std;

DBInMemory::~DBInMemory()
{
}

void DBInMemory::writeArticle(int group, const Article &article)
{
    auto it = checkGroups(group);
    checkList<Article>(it->articles, article.title, DBException{DBExceptionType::ARTICLE_ALREADY_EXISTS, " article exists"});

    int newarticleid = nextId<Article>(it->articles);
    Article newarticle{newarticleid, article.title, article.author, article.text};
    it->articles.push_back(newarticle);
}

const Article DBInMemory::readArticle(int group, int article)
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
    int newid = nextId<Group>(groups);
    Group newgroup{newid, title, vector<Article>{}};
    this->groups.push_back(newgroup);
}

const std::shared_ptr<vector<Article>> DBInMemory::readArticles(int group)
{
    auto it = checkGroups(group);
    return std::make_shared<vector<Article>>(it->articles);
}

void DBInMemory::deleteArticle(int group, int article)
{
    auto it = checkGroups(group);
    auto itart = checkArticles(it, article);
    it->articles.erase(itart);
}

void DBInMemory::deleteGroup(int group)
{
    auto it = checkGroups(group);
    groups.erase(it);
}

vector<Group>::iterator DBInMemory::checkGroups(int group)
{
    // binary search
    auto it = lower_bound(groups.begin(), groups.end(), group, [](const Group &g, int id) { return g.id < id; });
    if (it == groups.end() || it->id != group)
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, ""};
    }
    return it;
}

vector<Article>::iterator DBInMemory::checkArticles(vector<Group>::iterator &it, int article)
{
    // binary search
    auto itar = lower_bound(it->articles.begin(), it->articles.end(), article, [](const Article &a, int id) { return a.id < id; });
    if (itar == it->articles.end() || itar->id != article)
    {
        throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""};
    }
    return itar;
}