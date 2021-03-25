#include "dbdisk.h"

DBDisk::~DBDisk()
{
}

void DBDisk::writeArticle(ID group, const Article &article)
{
}

Article DBDisk::readArticle(ID group, ID article)
{
}

Groups DBDisk::readGroups()
{
}

void DBDisk::writeGroup(const string &name)
{
}

Articles DBDisk::readArticles(ID group)
{
}

void DBDisk::deleteArticle(ID group, ID article)
{
}

void DBDisk::deleteGroup(ID group)
{
}