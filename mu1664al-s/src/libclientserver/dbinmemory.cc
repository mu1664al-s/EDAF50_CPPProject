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
    auto it = find_if(groups.begin(), groups.end(), [group](const Group &g) { return g.id == group; });
    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, " group not found"};
    }
    else
    {
        int newarticleid = 1;
        if (it->articles.size() > 0)
        {
            newarticleid = it->articles.back().id + 1;
        }
        Article newarticle{newarticleid, article.title, article.author, article.text};
        it->articles.push_back(newarticle);
    }
}

const Article DBInMemory::readArticle(int group, int article)
{
    auto it = find_if(groups.begin(), groups.end(), [group](const Group &g) { return g.id == group; });

    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, " group not found"};
    }
    else
    {
        auto itart = find_if(it->articles.begin(), it->articles.end(), [article](const Article &a) { return a.id == article; });
        if (itart == it->articles.end())
        {
            throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, " article not found"};
        }
        else
        {
            return *itart;
        }
    }
}

const vector<Group> DBInMemory::readGroups()
{
    return this->groups;
}

void DBInMemory::writeGroup(const string &name)
{
    auto it = find_if(groups.begin(), groups.end(), [name](const Group &g) { return g.title == name; });
    if (it != groups.end())
    {
        throw DBException{DBExceptionType::GROUP_ALREADY_EXISTS, " group exists"};
    }
    int newid = 1;
    if (groups.size() > 0)
    {
        newid = groups.back().id + 1;
    }
    Group newgroup{newid, name, vector<Article>{}};
    this->groups.push_back(newgroup);
}

const vector<Article> DBInMemory::readArticles(int group)
{
    auto it = find_if(groups.begin(), groups.end(), [group](const Group &g) { return g.id == group; });
    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, " group not found"};
    }
    else
    {
        return it->articles;
    }
}

void DBInMemory::deleteArticle(int group, int article)
{
    auto it = find_if(groups.begin(), groups.end(), [group](const Group &g) { return g.id == group; });
    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, " group not found"};
    }
    else
    {
        auto itart = find_if(it->articles.begin(), it->articles.end(), [article](const Article &a) { return a.id == article; });
        if (itart == it->articles.end())
        {
            throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, " article not found"};
        }
        else
        {
            it->articles.erase(itart);
        }
    }
}

void DBInMemory::deleteGroup(int group)
{
    auto it = find_if(groups.begin(), groups.end(), [group](const Group &g) { return g.id == group; });
    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, " group not found"};
    }
    else
    {
        groups.erase(it);
    }
}