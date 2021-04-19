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

pair<int, string> readPair(const string &line)
{
    std::smatch sm;
    std::regex e("^([0-9]*)<>(.*)$");
    std::regex_match(line, sm, e);
    int id = std::stoi(sm.str(1));
    string title = sm.str(2);
    return make_pair(id, title);
}

void removeFromList(const string &fs_root, const string &path, int id)
{
    //remove item with id from list
    ifstream in_list;      //read from list file
    ofstream out_list_tmp; //write to a tmp list file
    string tmp_list_path = fs_root + "list_" + to_string(id) + "_tmp.txt";
    in_list.open(path, fstream::in);
    out_list_tmp.open(tmp_list_path, fstream::out);
    string line;
    pair<int, string> pair;
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

int titleInList(ifstream &in_list, const string &title, const DBException &e)
{
    string line;
    pair<int, string> pair;
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

void appendToList(const string &path, int id, const string &title)
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
    pair<int, string> pair;
    while (getline(in_list, line))
    {
        pair = readPair(line);
        list.push_back(T{pair.first, pair.second});
    }
    in_list.close();
}

int nextId(std::ifstream &in_list)
{
    string line;
    pair<int, string> pair;
    while (getline(in_list, line))
    {
        pair = readPair(line);
    }
    in_list.close();
    return pair.first + 1;
}

void DBDisk::writeArticle(int group, const Article &article)
{
    //check if group exists
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    ifstream in_group = checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""}, false);

    //adding article to group
    int next_id = nextId(in_group);
    appendToList(group_path, next_id, article.title);

    //writing the article to file
    string article_path = fs_root + "g" + to_string(group) + "a" + to_string(next_id) + ".txt";
    //look in outer namespace
    ::writeArticle(article_path, article);
}

const Article DBDisk::readArticle(int group, int article)
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
    int id = titleInList(in_groups, title, DBException{DBExceptionType::GROUP_ALREADY_EXISTS, ""});

    //write to news groups
    int next_id = id + 1;
    appendToList(groups_path, next_id, title);

    //create the group file
    ofstream out_group; //[gid.txt]
    out_group.open(fs_root + "g" + to_string(next_id) + ".txt", fstream::out);
    out_group.close();
}

const std::shared_ptr<vector<Article>> DBDisk::readArticles(int group)
{
    //read group
    vector<Article> articles;
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    ifstream in_group = checkFile(group_path, DBException{DBExceptionType::GROUP_NOT_FOUND, ""}, false);
    readList(in_group, articles);
    return std::make_shared<vector<Article>>(articles);
}

void DBDisk::deleteArticle(int group, int article)
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

void DBDisk::deleteGroup(int group)
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