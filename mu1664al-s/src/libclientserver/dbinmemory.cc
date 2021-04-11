#include "dbinmemory.h"
#include<iostream>
#include<vector>
#include<iterator>
#include<string>
#include<algorithm>
using namespace std;

DBInMemory::~DBInMemory()
{
}

void DBInMemory::writeArticle(int group, const Article &article)
{
    auto it = find_if(groups.begin(),groups.end(),[group](const Group& g){return g.id == group;});
    if(it == groups.end())
    {
            throw DBException{DBExceptionType::GROUP_NOT_FOUND," group not found"};
    }

    auto itart = find_if((*it).articles.begin(),(*it).articles.end(),[article](Article a){return a.name == article.name;});

    if (itart != (*it).articles.end())
    {
        throw DBExceptionType::ARTICLE_ALREADY_EXISTS;
    }
    else
    {
        if ((*it).articles.size() == 0)
        {
            int newarticleid = 1;
            Article newarticle{newarticleid,article.name,article.author,article.title,article.text};
            (*it).articles.emplace_back(newarticle);
        }
        else
        {
            int newarticleid = (*it).articles.back().id + 1;
            Article newarticle{newarticleid,article.name, article.author, article.title,article.text};
            (*it).articles.emplace_back(newarticle);
        }
    }
}

const Article DBInMemory::readArticle(int group, int article)
{
    auto it = find_if(groups.begin(),groups.end(),[group](const Group& g){return g.id==group;});

    if (it == groups.end())
    {
            throw DBExceptionType::GROUP_NOT_FOUND;
    }
    else
    {
        auto itart = find_if((*it).articles.begin(), (*it).articles.end(), [article](Article a){ return a.id == article; });
        if(itart == (*it).articles.end())
        {
            throw DBExceptionType::ARTICLE_NOT_FOUND;
        }
        else
        {
            return *itart;
        }
    }

}

const vector<Group> DBInMemory::readGroups()
{
    return this-> groups;
}

void DBInMemory::writeGroup(const string& name)
{
    if(groups.size()==0)
    {
        int newid = 1;
        Group newgroup{newid,name};
        this->groups.push_back(newgroup);
    }
    else
    {
    int newid = groups.back().id+1;
    Group newgroup{newid, name};
    this->groups.push_back(newgroup);
    }
}

const vector<Article> DBInMemory::readArticles(int group)
{
    auto it = find_if(groups.begin(),groups.end(),[group](Group g){return g.id == group;});
    if (it == groups.end())
    {
        throw DBExceptionType::GROUP_NOT_FOUND;
    }
    else
    {
        return (*it).articles;
    }
}

void DBInMemory::deleteArticle(int group, int article)
{
    bool foundarticle = false;
    auto it = find_if(groups.begin(),groups.end(),[group](Group g){return g.id == group;});
    if (it == groups.end())
    {
        throw DBExceptionType::GROUP_NOT_FOUND;
    }
    else
    {
        auto itart= find_if((*it).articles.begin(),(*it).articles.end(),[article](Article a){return a.id == article;});
        if(itart == (*it).articles.end())
        {
            throw DBExceptionType::ARTICLE_NOT_FOUND;
        }
        else
        {
            (*it).articles.erase(itart);
        }
    }

}

void DBInMemory::deleteGroup(int group)
{


    auto it = find_if(groups.begin(),groups.end(),[group](Group g){return g.id == group;});
    if (it == groups.end())
    {
        throw DBExceptionType::GROUP_NOT_FOUND;
    }
    else
    {
        groups.erase(it);
    }

}
int DBInMemory::getsize()
{
    return groups.size();
}