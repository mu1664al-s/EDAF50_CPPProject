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
DBDisk::DBDisk(const string &fs_root)
{
    this->fs_root = fs_root;
    //create newsgroup.tx if it doesn't exists
    ofstream out_register;
    out_register.open(fs_root + "newsgroup.txt", fstream::out);
    out_register.close();
}
DBDisk::~DBDisk()
{
}

ifstream checkFile(const string &path, const DBException &e, bool closeAfter = true)
{
    ifstream in_file;
    in_file.open(path, fstream::in);
    if (!in_file.is_open())
    {
        throw e;
    }
    if (closeAfter)
    {
        in_file.close();
    }
    return in_file;
}

inline void writeArticle(const string &path, const Article &article)
{
    ofstream out_article;
    out_article.open(path, fstream::out);
    out_article << article.title << endl;
    out_article << article.author << endl;
    istringstream ss(article.text);
    string text_line;
    getline(ss, text_line);
    out_article << text_line;
    while (getline(ss, text_line))
    {
        out_article << "\n"
                    << text_line;
    }
    out_article.close();
}

inline void readArticle(const string &path, Article &article)
{
    ifstream in_article = checkFile(path, DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""}, false);
    getline(in_article, article.title);
    getline(in_article, article.author);
    string text_line = "";
    getline(in_article, article.text);
    while (getline(in_article, text_line))
    {
        article.text += "\n" + text_line;
    }
    if (!(text_line == ""))
    {
        article.text += "\n";
    }
    in_article.close();
}

pair<size_t, string> readPair(const string &line)
{
    std::smatch sm;
    std::regex e("^([0-9]*)<>(.*)$");
    std::regex_match(line, sm, e);
    size_t id = std::stoi(sm.str(1));
    string title = sm.str(2);
    return make_pair(id, title);
}

void removeFromList(const string &fs_root, const string &path, size_t id)
{
    //remove item with id from list
    ifstream in_list;      //read from list file
    ofstream out_list_tmp; //write to a tmp list file
    string tmp_list_path = fs_root + "list_" + to_string(id) + "_tmp.txt";
    in_list.open(path, fstream::in);
    out_list_tmp.open(tmp_list_path, fstream::out);
    string line;
    pair<size_t, string> pair;
    if (in_list.is_open() && out_list_tmp.is_open())
    {
        while (getline(in_list, line))
        {
            pair = ::readPair(line);
            if (pair.first != id)
            {
                out_list_tmp << line << endl;
            }
        }
        in_list.close();
        out_list_tmp.close();
        remove(path.c_str());
        rename(tmp_list_path.c_str(), path.c_str());
    }
}

size_t titleInList(ifstream &in_list, const string &title, const DBException &e)
{
    string line;
    pair<size_t, string> pair;
    while (getline(in_list, line))
    {
        pair = ::readPair(line);
        if (pair.second == title)
        {
            in_list.close();
            throw e;
        }
    }
    in_list.close();
    return pair.first;
}

void appendToList(const string &path, size_t id, const string &title)
{
    //append to list file
    ofstream out_list;
    out_list.open(path, fstream::app);
    out_list << id << "<>" << title << endl;
    out_list.close();
}

template <typename T>
void readList(std::ifstream &in_list, vector<T> &list)
{
    string line;
    pair<size_t, string> pair;
    while (getline(in_list, line))
    {
        pair = readPair(line);
        list.push_back(T{pair.first, pair.second});
    }
    in_list.close();
}

void DBDisk::writeArticle(size_t group, const Article &article)
{
    //check in the group file for title
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    ifstream in_group = checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""}, false);
    size_t id = titleInList(in_group, article.title, DBException{DBExceptionType::ARTICLE_ALREADY_EXISTS, ""});

    //adding article to group
    size_t next_id = id + 1;
    appendToList(group_path, next_id, article.title);

    //writing the article to file
    string article_path = fs_root + "g" + to_string(group) + "a" + to_string(next_id) + ".txt";
    //look in outer namespace
    ::writeArticle(article_path, article);
}

const Article DBDisk::readArticle(size_t group, size_t article)
{
    //check if group exists
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""});

    //read article
    string article_path = fs_root + "g" + to_string(group) + "a" + to_string(article) + ".txt";
    Article _article = Article{};
    //look in outer namespace
    ::readArticle(article_path, _article);

    return _article;
}

const std::shared_ptr<vector<Group>> DBDisk::readGroups()
{
    //read groups file
    vector<Group> groups;
    ifstream in_register;
    in_register.open(fs_root + "newsgroup.txt", fstream::in);
    readList(in_register, groups);
    return std::make_shared<vector<Group>>(groups);
}

void DBDisk::writeGroup(const string &title)
{
    //check news groups
    ifstream in_groups;
    string groups_path = fs_root + "newsgroup.txt";
    in_groups.open(groups_path, fstream::in);
    size_t id = titleInList(in_groups, title, DBException{DBExceptionType::GROUP_ALREADY_EXISTS, ""});

    //write to news groups
    size_t next_id = id + 1;
    appendToList(groups_path, next_id, title);

    //create the group file
    ofstream out_group; //[gid.txt]
    out_group.open(fs_root + "g" + to_string(next_id) + ".txt", fstream::out);
    out_group.close();
}

const std::shared_ptr<vector<Article>> DBDisk::readArticles(size_t group)
{
    //read group
    vector<Article> articles;
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    ifstream in_group = checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""}, false);
    readList(in_group, articles);
    return std::make_shared<vector<Article>>(articles);
}

void DBDisk::deleteArticle(size_t group, size_t article)
{
    //check if group exists
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""});

    //check if article file exists
    string article_path = fs_root + "g" + to_string(group) + "a" + to_string(article) + ".txt";
    checkFile(article_path, DBException{DBExceptionType::ARTICLE_NOT_FOUND, ""});

    //delete article
    remove(article_path.c_str()); //remove the article document.

    //remove article from group
    removeFromList(fs_root, group_path, article);
}

void DBDisk::deleteGroup(size_t group)
{
    //check if group exists
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    ifstream in_group = checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""}, false);

    //remove articles that belong to the group
    pair<int, string> pair;
    string line;
    string article_path;
    while (getline(in_group, line))
    {
        pair = readPair(line);
        //delete article
        article_path = fs_root + "g" + to_string(group) + "a" + to_string(pair.first) + ".txt";
        remove(article_path.c_str()); //remove the article document.
    }
    in_group.close();

    //remove the group file
    remove(group_path.c_str());

    //remove group from news groups
    string groups_path = fs_root + "newsgroup.txt";
    removeFromList(fs_root, groups_path, group);
}