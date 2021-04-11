#include "dbdisk.h"

DBDisk::~DBDisk()
{
}

void DBDisk::writeArticle(int group, const Article &article)
{
}

const Article DBDisk::readArticle(int group, int article)
{
}

const vector<Group> DBDisk::readGroups()
{
}

void DBDisk::writeGroup(const string &name)
{
}

const vector<Article> DBDisk::readArticles(int group)
{
    return vector<Article>(); // placeholder
}

void DBDisk::deleteArticle(int group, int article)
{
}

void DBDisk::deleteGroup(int group)
{
}