
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string>
#include <vector>

using std::string;
using std::vector;

using ID = unsigned int;

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
    ID id;       // unique
    string name; // unique
    Articles articles;
};

using Groups = vector<Group>;

#endif
