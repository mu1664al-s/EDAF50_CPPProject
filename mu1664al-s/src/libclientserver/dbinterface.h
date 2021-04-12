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
	int id; // unique
	string title;
	string author;
	string text;
};

struct Group
{
	int id;		  // unique
	string title; // unique
	vector<Article> articles;
};

enum class DBExceptionType
{
	GROUP_NOT_FOUND = 0,
	GROUP_ALREADY_EXISTS = 1,
	ARTICLE_NOT_FOUND = 2,
	ARTICLE_ALREADY_EXISTS = 3
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

	virtual void writeArticle(int group, const Article &article) = 0; //write a new article to a newsgroup

	virtual const Article readArticle(int group, int article) = 0; // read an article from a newsgroup, using group identity and article identity

	virtual const vector<Group> readGroups() = 0;

	virtual void writeGroup(const string &name) = 0; //

	virtual const vector<Article> readArticles(int group) = 0; // get articles from a newsgroup, using the group identity

	virtual void deleteArticle(int group, int article) = 0; //delete an article, specified by its id in a newsgroup.

	virtual void deleteGroup(int group) = 0; //deletegroup, using the group identity.
};

#endif
