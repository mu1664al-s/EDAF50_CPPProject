#include "dbinmemory.h"

DBInMemory::~DBInMemory()
{
}

void DBInMemory::writeArticle(ID group, const Article &article)
{
}

Article DBInMemory::readArticle(ID group, ID article)
{
}

Groups DBInMemory::readGroups()
{
}

void DBInMemory::writeGroup(const string &name)
{
}

Articles DBInMemory::readArticles(ID group)
{
}

void DBInMemory::deleteArticle(ID group, ID article)
{
}

void DBInMemory::deleteGroup(ID group)
{
}