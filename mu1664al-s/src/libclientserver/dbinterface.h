/*
 * Interface DBInterface -- Defines communication with the database
 */
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Article
{
	const int id; // unique
	const string name;
	const string author;
	const string title;
	const string text;
};

struct Group
{
	const int id;	   // unique
	const string name; // unique
	const vector<Article> articles;
};

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

	virtual void writeArticle(int group, const Article &article) = 0;

	virtual const Article readArticle(int group, int article) = 0;

	virtual const vector<Group> readGroups() = 0;

	virtual void writeGroup(const string &name) = 0;

	virtual const vector<Article> readArticles(int group) = 0;

	virtual void deleteArticle(int group, int article) = 0;

	virtual void deleteGroup(int group) = 0;
};

#endif
