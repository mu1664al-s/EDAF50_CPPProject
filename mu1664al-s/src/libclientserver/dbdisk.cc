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
    ifstream in_register;
    pair<int, string> pair;
    string line;
    in_register.open(fs_root + "newsgroup.txt", fstream::out | fstream::in);
    while (getline(in_register, line))
    {
        pair = readPair(line);
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
    in_group.open(fs_root + "g" + to_string(group) + ".txt", fstream::in);
    string line;
    pair<int, string> pair;
    while (getline(in_group, line))
    {
        pair = readPair(line);
        if (pair.second == article.title)
        {
            throw DBException{DBExceptionType::ARTICLE_ALREADY_EXISTS, ""};
        }
    }
    in_group.close();

    // defined output
    int next_id = pair.first + 1;

    //adding article to group
    ofstream out_group;
    out_group.open(fs_root + "g" + to_string(group) + ".txt", fstream::app | fstream::out);
    out_group << next_id << "<>" << article.title << endl;
    ofstream out_article;
    out_group.close();

    //writing the article to file
    out_article.open(fs_root + "g" + to_string(group) + "a" + to_string(next_id) + ".txt", fstream::out);
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

const Article DBDisk::readArticle(int group, int article)
{
    //check register
    checkRegister(group);

    // check the group
    ifstream in_group;
    in_group.open(fs_root + "g" + to_string(group) + ".txt", fstream::in);
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
    in_article.open(fs_root + "g" + to_string(group) + "a" + to_string(article) + ".txt", fstream::in);
    string author, text;
    getline(in_article, author);
    string text_line = "";
    getline(in_article, text);
    while (getline(in_article, text_line))
    {
        text += "\n" + text_line;
    }
    if (!(text_line == ""))
    {
        text += "\n";
    }
    in_article.close();

    return Article{pair.first, pair.second, author, text};
}

const vector<Group> &DBDisk::readGroups()
{
    return groups;
}

void DBDisk::writeGroup(const string &title)
{
    //check the register after group name
    auto it = find_if(groups.begin(), groups.end(), [&title](const Group &group) { return group.title == title; });
    if (it != groups.end())
    {
        throw DBException{DBExceptionType::GROUP_ALREADY_EXISTS, ""};
    }

    //write to news groups
    ofstream out_register; // newsgorup.txt
    ofstream out_group;    //[gid.txt]
    out_register.open(fs_root + "newsgroup.txt", fstream::app);
    int id = 1;
    if (groups.size() > 0)
    {
        id = groups.back().id + 1;
    }
    out_register << id << "<>" << title << endl;
    out_register.close();

    //add to register
    groups.push_back(Group{id, title, vector<Article>{}});

    //create the group file
    out_group.open(fs_root + "g" + to_string(id) + ".txt", fstream::out);
    out_group.close();
}

const vector<Article> DBDisk::readArticles(int group)
{
    //check register
    checkRegister(group);

    //read group
    vector<Article> articles;
    ifstream in_group;
    in_group.open(fs_root + "g" + to_string(group) + ".txt", fstream::in);
    string line;
    pair<int, string> pair;
    while (getline(in_group, line))
    {
        pair = readPair(line);
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
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    out_group.open(group_path, fstream::trunc);
    for (Article a : articles)
    {
        out_group << a.id << "<>" << a.title << endl;
    }
    out_group.close();

    //delete article
    string article_path = fs_root + "g" + to_string(group) + "a" + to_string(article) + ".txt";
    remove(article_path.c_str()); //remove the article document.
}

void DBDisk::deleteGroup(int group)
{
    //check register
    auto it = checkRegister(group);

    //read group
    vector<Article> articles = readArticles(group);

    //delete group file
    string group_path = fs_root + "g" + to_string(group) + ".txt";
    remove(group_path.c_str());

    //delete articles
    for (Article article : articles)
    {
        string article_path = fs_root + "g" + to_string(group) + "a" + to_string(article.id) + ".txt";
        remove(article_path.c_str()); //remove the article document.
    }

    //delete group from register
    groups.erase(it);
    ofstream out_groups;
    string groups_path = fs_root + "newsgroup.txt";
    out_groups.open(groups_path, fstream::trunc);
    string title;
    for (Group g : groups)
    {
        out_groups << g.id << "<>" << g.title << endl;
    }
    out_groups.close();
}

vector<Group>::iterator DBDisk::checkRegister(int group)
{
    // binary search
    auto it = lower_bound(groups.begin(), groups.end(), group, [](const Group &g, int id) { return g.id < id; });
    if (it == groups.end() || it->id != group)
    {
        throw DBException{DBExceptionType::GROUP_NOT_FOUND, ""};
    }
    return it;
}

pair<int, string> DBDisk::readPair(const string &line)
{
    std::smatch sm;
    std::regex e("^([0-9]*)<>(.*)$");
    std::regex_match(line, sm, e);
    int id = std::stoi(sm.str(1));
    string title = sm.str(2);
    return make_pair(id, title);
}