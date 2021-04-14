#include"dbinterface.h"
#include "dbdisk.h"
#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<stdio.h>
#include<regex>
using namespace std;
string path = "/mnt/c/Users/co7647ng/Program/VisualStudioProjects/C++Course/Project/EDAF50_CPPProject/mu1664al-s/src/libclientserver/database/";
DBDisk::DBDisk()
{
ifstream infile;
int id;
string tmp;
char delim;
string line;
infile.open(path + "newsgroup.txt");
while(getline(infile, line))
{
    istringstream ss(line);
    ss >> id >> delim >> delim >> tmp;
    groupids.emplace_back(id);
    groupnames.emplace_back(tmp);
}
}
DBDisk::~DBDisk()
{
}

void DBDisk::writeArticle(int group, const Article &article)
{  
  auto it = find_if(groupids.begin(),groupids.end(),[&group](int id){return id == group;});
  if(it== groupids.end())
  {
      cout << "GROUP_NOT_FOUND" ;
  }
  else
  {
      ifstream infile;
      infile.open(path + "g" + to_string(group) + ".txt");
      vector<string> articlenames;
      vector<int> articleids;
      string line;
      string name;
      int id;
      char delim;
      while(getline(infile,line))
      {
          stringstream ss(line);
          ss>>id>>delim>>delim>> name;
          articlenames.emplace_back(name);
          articleids.emplace_back(id);
      }
      infile.close();
      if(articleids.size()==0) //check for empty group
      {
          ofstream outfile; // append the article name and id to gid.txt
          outfile.open(path + "g" + to_string(group) + ".txt",ios::app);
          outfile << 1 << "<>" << article.name << endl; 
          ofstream outfile1;
          outfile1.open(path + "g" +to_string(group) + "a" + to_string(1) + ".txt"); //create new gidaid.txt;
		  string text;
		  text = regex_replace(article.text, regex("\n"), "\\n" );
          outfile1 << 1 <<endl<< article.name << endl << article.author << endl << article.title << endl<< text << endl; 
      }
      else // if the group is not empty, check the existed article names.
      {
        auto itart = find_if(articlenames.begin(),articlenames.end(),[&article](string s){return s == article.name;});
        if(itart !=articlenames.end())
        {
            cout << "ARTICLE_ALREADY_EXISTS";
        }
        else
        {
          ofstream outfile; 
          outfile.open(path + "g" + to_string(group) + ".txt",ios::app);
          int newid = articleids.back()+1;
          outfile << newid << "<>" << article.name << endl;
          ofstream outfile1; 
          outfile1.open(path + "g" +to_string(group) + "a" + to_string(newid) + ".txt");
		  string text;
		  text = regex_replace(article.text, regex("\n"), "\\n");
          outfile1 <<newid << endl <<article.name << endl << article.author << endl << article.title << endl << text << endl; 
        }

      }
    }
}

const Article DBDisk::readArticle(int group, int article)
{
  vector<int> articleids;  
  auto it = find_if(groupids.begin(),groupids.end(),[&group](int id){return id == group;});
  if(it== groupids.end()) // check for existed group
  {
      cout << "GROUP_NOT_FOUND" << endl;
      Article res;
      return res;
  }
  else
  {
      ifstream infile;
      infile.open(path + "g" + to_string(group) + ".txt");
      string line;
      int id;
      while(getline(infile,line))
      {
          stringstream ss(line);
          ss>>id;
          articleids.emplace_back(id);
      }
      infile.close();
  }
  auto itart = find_if(articleids.begin(),articleids.end(),[&article](int i){return i == article;});
  if(itart == articleids.end())
  {
      cout << "ARTICLE_NOT_FOUND"<<endl;
      Article res;
      return res;
  }
  else
  {
      int id = *itart;
      ifstream infile1;
      infile1.open(path + "g" + to_string(group) + "a" + to_string(id) +".txt");
      string skip,name, author, title, text;
      getline(infile1,skip); // skip the id
      getline(infile1,name);
      getline(infile1,author);
      getline(infile1,title);
      getline(infile1,text);
    
	  text = regex_replace(text, regex("\\\\n"), "\n");
      Article res{id,name,author,title,text};
      return res;
  }
  
}

const vector<Group> DBDisk::readGroups()
{
	vector<Group> groups;
	if (groupids.size() == 0)
	{
		cout << "GROUP_NOT_FOUND";
	}
	else
	{
		for (int i = 0; i < groupids.size(); i++)
		{	
			vector<Article> articles = readArticles(groupids[i]);
			Group group{groupids[i],groupnames[i],articles};
			groups.push_back(group);
		}
	}
	return groups;
}

void DBDisk::writeGroup(const string &name)
{
    auto it = find_if(groupnames.begin(),groupnames.end(), [&name](string s){return s == name;});
    if(it != groupnames.end())
    {
        cout << "GROUP_ALREADY_EXISTS";
    }
    else
    {
        ofstream outfile; // newsgorup.txt
        ofstream outfile1; //[gid.txt]
        outfile.open(path + "newsgroup.txt", ios::app);
        if(groupids.size()==0)
        {
            outfile<< 1 << "<>" << name << endl;
            groupids.emplace_back(1);
            groupnames.emplace_back(name);
            outfile1.open(path+"g"+ to_string(1) + ".txt");
        }
        else
        {
            int newid = groupids.back()+1;
            outfile<< newid << "<>" << name << endl;
            groupids.emplace_back(newid);
            groupnames.emplace_back(name);
            outfile1.open(path+"g"+ to_string(newid) + ".txt");
        }
    }
}

const vector<Article> DBDisk::readArticles(int group)
{   
    vector<Article> res;
    vector<int> articleids;
    auto it =find_if(groupids.begin(),groupids.end(),[&group](int id){return id == group;});
    if(it==groupids.end())
    {
        cout << "GROUP_NOT_FOUND" <<endl;
    }
    else
    {
        ifstream infile;
        infile.open(path + "g" + to_string(group)+ ".txt");
        string line;
        int id;
        while(getline(infile,line))
        {
            stringstream ss(line);
            ss>>id;
            articleids.emplace_back(id);
        } 
        infile.close();
    }
    if(articleids.size()==0)
    {
        cout << "ARTICLE_NOT_FOUND" <<endl;
        return res;
    }
    else
    {
        for(int i = 0; i < articleids.size();i++)
        {
            auto article = readArticle(group,articleids[i]);
            res.emplace_back(article);
        }
        return res;
    }
}

void DBDisk::deleteArticle(int group, int article)
{
    auto it= find_if(groupids.begin(),groupids.end(),[&group](int id){return id == group;});
    if(it == groupids.end())
    {
        cout << "GROUP_NOT_FOUND";
    }
    else
    {
        ifstream infile;//open gid.txt
        ofstream outfile;//create a tmp.txt, later rename to gid.txt
        string groupfilename = path + "g" + to_string(group) + ".txt";
        string articlefilename = path + "g" + to_string(group) + "a" + to_string(article) + ".txt";
        string tmp = path + "tmp.txt";
        infile.open(groupfilename);
        outfile.open(tmp);
        string line;
        int id;
        while(getline(infile,line))
        {
            stringstream ss(line);
            ss >> id;
            if(id != article)
            {
                outfile << line << endl;
            }
        }
        infile.close();
        outfile.close();
        remove(groupfilename.c_str()); // remove the old gid.txt
        remove(articlefilename.c_str()); //remove the article document. 
        rename(tmp.c_str(), groupfilename.c_str());
        
    }
}

void DBDisk::deleteGroup(int group)
{   
    vector<int> articleids;
    int id;
    auto it = find_if(groupids.begin(),groupids.end(),[&group](int i){return i == group;});
    if(it == groupids.end())
    {
        cout << "GROUP_NOT_FOUND" << endl;
    }
    else
    {
        groupids.erase(remove(groupids.begin(),groupids.end(), group),groupids.end()); //remove id from groupids.
        string filename = path + "g" + to_string(group) + ".txt";
        string newsgroups = path + "newsgroup.txt";
        string tmp = path + "tmp.txt";
        ifstream infile; // for the gid.txt
        ifstream infile1; //for newsgroup.txt
        ofstream outfile; // to write a new newsgroup.txt
        infile.open(filename);
        string line;
        while(getline(infile,line))
        {
            stringstream ss(line);
            ss >> id;
            articleids.emplace_back(id);
        }
        infile.close();
        remove(filename.c_str());
        for(int i = 0; i <articleids.size();i++)
        {
            string articlefilename = path + "g" + to_string(group) + "a" + to_string(articleids[i]) + ".txt";
            remove(articlefilename.c_str()); //remove article documents.
        }
        infile1.open(newsgroups); // delete the group info in newsgroup.txt
        outfile.open(tmp);
        while(getline(infile1,line))
        {
            stringstream ss(line);
            ss >> id;
            if(id != group)
            outfile << line << endl;
        }
        infile1.close();
        outfile.close();
        remove(newsgroups.c_str());
        rename(tmp.c_str(),newsgroups.c_str());
    }
}
void DBDisk::getids()
{
    for(int i = 0; i < groupids.size();i++)
    {
        cout << groupids[i] << " ";
    }
    cout << endl;
}
void DBDisk::getnames()
{
    for(int i =0; i < groupnames.size();i++)
    {
        cout << groupnames[i] << "----";
    }
    cout << endl;
}