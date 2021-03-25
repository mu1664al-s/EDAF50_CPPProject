/*
 * Interface DBInterface -- Defines communication with the database
 */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

using ID = int;

struct Article
{
	ID id; // unique
	string name;
	string author;
	string title;
	string text;
};

using Articles = vector<Article>;

struct Group
{
	ID id;		 // unique
	string name; // unique
	Articles articles;
};

using Groups = vector<Group>;

enum class DBExceptionType
{
	GROUP_NOT_FOUND = 0,
	GROUP_ALREADY_EXITS = 1,
	ARTICLE_NOT_FOUND = 2
};

struct DBException
{
	DBExceptionType type;
	string message;
};

class DBInterface
{
public:
	virtual ~DBInterface() = default;

	virtual void writeArticle(ID group, const Article &article) = 0;

	virtual Article readArticle(ID group, ID article) = 0;

	virtual Groups readGroups() = 0;

	virtual void writeGroup(const string &name) = 0;

	virtual Articles readArticles(ID group) = 0;

	virtual void deleteArticle(ID group, ID article) = 0;

	virtual void deleteGroup(ID group) = 0;
};

#endif
