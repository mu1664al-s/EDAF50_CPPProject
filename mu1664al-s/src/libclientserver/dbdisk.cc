#include "dbinterface.h"
#include "dbdisk.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <regex>
using namespace std;
string path = "/home/sandbox/git/EDAF50_CPPProject/mu1664al-s/src/libclientserver/database/";
DBDisk::DBDisk()
{
    ifstream in_register;
    pair<int, string> pair;
    string line;
    in_register.open(path + "newsgroup.txt", fstream::out | fstream::in);
    while (getline(in_register, line))
    {
        pair = readPair(line, true);
        groups.push_back(Group{pair.first, pair.second, vector<Article>{}});
    }
    in_register.close();
}
DBDisk::~DBDisk()
{
}

void DBDisk::writeArticle(int group, const Article &article)
{
    //check register
    checkRegister(group);

    ifstream in_group;
    in_group.open(path + "g" + to_string(group) + ".txt", fstream::in);
    string line;
    pair<int, string> pair;
    while (getline(in_group, line))
    {
        pair = readPair(line, true);
        if (pair.second == article.title)
        {
            throw DBException{DBExceptionType::ARTICLE_ALREADY_EXISTS, ""};
        }
    }
    in_group.close();

    // defined output
    int next_id = pair.first + 1;
    string title = regex_replace(article.title, regex(" "), "\\\\s");
    string author = regex_replace(article.author, regex(" "), "\\\\s");
    string text = regex_replace(article.text, regex(" "), "\\\\s");
    text = regex_replace(text, regex("\n+"), "\\n");

    //adding article to group
    ofstream out_group;
    out_group.open(path + "g" + to_string(group) + ".txt", fstream::app | fstream::out);
    out_group << next_id << "<>" << title << endl;
    ofstream out_article;
    out_group.close();

    //writing the article to file
    out_article.open(path + "g" + to_string(group) + "a" + to_string(next_id) + ".txt", fstream::out);
    out_article << author << endl
                << text << endl;
    out_article.close();
}

const Article DBDisk::readArticle(int group, int article)
{
    //check register
    checkRegister(group);

    // check the group
    ifstream in_group;
    in_group.open(path + "g" + to_string(group) + ".txt", fstream::in);
    string line;
    pair<int, string> pair;
    bool article_found = false;
    while (getline(in_group, line))
    {
        pair = readPair(line);
        if (pair.first == article)
        {
            article_found = true;
            break;
        }
    }
    in_group.close();

    //check if article found in group
    if (!article_found)
    {
        throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""};
    }

    //read article
    ifstream in_article;
    in_article.open(path + "g" + to_string(group) + "a" + to_string(article) + ".txt", fstream::in);
    string author, text;
    getline(in_article, author);
    getline(in_article, text);

    //create article structure
    string title = regex_replace(pair.second, regex("\\\\\\\\s"), " ");
    author = regex_replace(author, regex("\\\\\\\\s"), " ");
    text = regex_replace(text, regex("\\\\n"), "\n");
    text = regex_replace(text, regex("\\\\\\\\s"), " ");
    in_article.close();

    return Article{pair.first, title, author, text};
}

const vector<Group> DBDisk::readGroups()
{
    return groups;
}

void DBDisk::writeGroup(const string &name)
{
    //check the register after group name
    auto it = find_if(groups.begin(), groups.end(), [&name](const Group &group) { return group.title == name; });
    if (it != groups.end())
    {
        throw DBException{DBExceptionType::GROUP_ALREADY_EXISTS, ""};
    }

    //write to news groups
    string title = regex_replace(name, regex(" "), "\\\\s");
    ofstream out_register; // newsgorup.txt
    ofstream out_group;    //[gid.txt]
    out_register.open(path + "newsgroup.txt", fstream::app);
    int id = 1;
    if (groups.size() > 0)
    {
        id = groups.back().id + 1;
    }
    out_register << id << "<>" << title << endl;
    out_register.close();

    //add to register
    groups.push_back(Group{id, name, vector<Article>{}});

    //create the group file
    out_group.open(path + "g" + to_string(id) + ".txt", fstream::out);
    out_group.close();
}

const vector<Article> DBDisk::readArticles(int group)
{
    //check register
    checkRegister(group);

    //read group
    vector<Article> articles;
    ifstream in_group;
    in_group.open(path + "g" + to_string(group) + ".txt", fstream::in);
    string line;
    pair<int, string> pair;
    while (getline(in_group, line))
    {
        pair = readPair(line, true);
        articles.push_back(Article{pair.first, pair.second, "", ""});
    }
    in_group.close();
    return articles;
}

void DBDisk::deleteArticle(int group, int article)
{
    //check register
    checkRegister(group);

    //find article in group
    vector<Article> articles = readArticles(group);
    auto article_it = find_if(articles.begin(), articles.end(), [&article](const Article &a) { return a.id == article; });
    if (article_it == articles.end())
    {
        throw DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""};
    }

    //delete article in group
    articles.erase(article_it);
    ofstream out_group; //create a tmp.txt, later rename to gid.txt
    string group_path = path + "g" + to_string(group) + ".txt";
    out_group.open(group_path, fstream::trunc);
    string title;
    for (Article a : articles)
    {
        title = regex_replace(a.title, regex(" "), "\\\\s");
        out_group << a.id << "<>" << title << endl;
    }
    out_group.close();

    //delete article
    string article_path = path + "g" + to_string(group) + "a" + to_string(article) + ".txt";
    remove(article_path.c_str()); //remove the article document.
}

void DBDisk::deleteGroup(int group)
{
    //check register
    auto it = checkRegister(group);

    //read group
    vector<Article> articles = readArticles(group);

    //delete group file
    string group_path = path + "g" + to_string(group) + ".txt";
    remove(group_path.c_str());

    //delete articles
    for (Article article : articles)
    {
        string article_path = path + "g" + to_string(group) + "a" + to_string(article.id) + ".txt";
        remove(article_path.c_str()); //remove the article document.
    }

    //delete group from register
    groups.erase(it);
    ofstream out_groups;
    string groups_path = path + "newsgroup.txt";
    out_groups.open(groups_path, fstream::trunc);
    string title;
    for (Group g : groups)
    {
        title = regex_replace(g.title, regex(" "), "\\\\s");
        out_groups << g.id << "<>" << title << endl;
    }
    out_groups.close();
}

vector<Group>::iterator DBDisk::checkRegister(int group)
{
    auto it = find_if(groups.begin(), groups.end(), [&group](const Group &g) { return g.id == group; });
    if (it == groups.end())
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, ""};
    }
    return it;
}

pair<int, string> DBDisk::readPair(const string &line, bool escape) const
{
    int id;
    string title;
    char delim;
    istringstream ss(line);
    ss >> id >> delim >> delim >> title;
    if (escape)
    {
        title = regex_replace(title, regex("\\\\\\\\s"), " ");
    }
    return make_pair(id, title);
}