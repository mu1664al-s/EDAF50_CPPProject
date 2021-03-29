#include "dbinmemory.h"

DBInMemory::~DBInMemory()
{
}

void DBInMemory::writeArticle(int group, const Article &article)
{
}

const Article DBInMemory::readArticle(int group, int article)
{
}

const vector<Group> DBInMemory::readGroups()
{
}

void DBInMemory::writeGroup(const string &name)
{
}

const vector<Article> DBInMemory::readArticles(int group)
{
}

void DBInMemory::deleteArticle(int group, int article)
{
}

void DBInMemory::deleteGroup(int group)
{
}