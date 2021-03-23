/*
 * Interface DBInterface -- Defines communication with the database
 */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include "datastructures.h"

enum class DBExceptionType
{
	GROUP_NOT_FOUND = 0,
	ARTICLE_NOT_FOUND = 1,
};

struct DBException
{
	int type;
	string message;
};

class DBInterface
{
public:
	virtual ~DBInterface() = default;

	virtual bool writeArticle(ID group, Article article) = 0;

	virtual Article readArticle(ID group, ID article) = 0;

	virtual Groups readGroups() = 0;

	virtual void writeGroup(string name) = 0;

	virtual Articles readArticles(ID group) = 0;

	virtual bool deleteArticle(ID group, ID article) = 0;

	virtual bool deleteGroup(ID group) = 0;
};

#endif