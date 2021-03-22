
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Article
{
    unsigned int id; // unique
    string name;
    string author;
    string title;
    string text;
};

using Articles = vector<Article>;

struct Group
{
    unsigned int id; // unique
    string name;     // unique
    Articles articles;
};

using Groups = vector<Group>;

#endif
