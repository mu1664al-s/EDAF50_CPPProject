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

	/*
	 * Insert a name/address pair. Does not check if the name
	 * or address already exists.
	 */
	virtual bool writeArticle(Group group, Article article) = 0;

	/*
	 * Insert a name/address pair. Does not check if the name
	 * or address already exists.
	 */
	virtual Article readArticle(Group group, Article article) = 0;
};

#endif
