#include"dbinmemory.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;

void printGroups(const vector<Group>& groups)
{
   for(auto it = groups.begin(); it!= groups.end();it++)
   {
      cout << (*it).id << " " << (*it).name<< endl;
   }
}
void printArticle(const Article& a)
{
   cout << a.id << " " << a.name << " " << a.author << " " << a.title << " " << a.text << " " << endl;
}
void printArticles(const vector<Article>& articles)
{
   for(auto it=articles.begin();it!=articles.end();it++)
   {
      printArticle(*it);
   }
}

int main()
{
   //init database, test write Group, test write Article.
  DBInMemory mydb;
  cout << "test writeGroup and readGroups:" << endl;
  mydb.writeGroup("group1");
  mydb.writeGroup("group2");
  mydb.writeGroup("group3");
  auto testreadgroup = mydb.readGroups();
  printGroups(testreadgroup);
  cout << endl;
  Article article1{0,"article1","vu","c project", "first"}; // set 0 as a default id for an article, mydb will assign another db for the added article.
  Article article2{0,"article2","vu","c project", "second"};
  Article article3{0,"article3","vu","c project", "third"};
  cout << "test writeArticle and readArticles:"<< endl;
  mydb.writeArticle(1,article1);
  mydb.writeArticle(1,article2);
  mydb.writeArticle(2,article3);
  cout << "first group" << endl;
  auto testreadarticles1 = mydb.readArticles(1);
  printArticles(testreadarticles1);
  cout << "second group" << endl;
  auto testreadarticles2 = mydb.readArticles(2);
  printArticles(testreadarticles2);
  cout << endl;
  cout << "test readArticle by finding the second article in group1:" << endl;
  auto testreadarticle = mydb.readArticle(1,2);
  printArticle(testreadarticle);
  cout << endl;
  cout << "test deleteArticle by delete article second from the first group:" << endl;
  mydb.deleteArticle(1,2);
  auto testdeletearticle = mydb.readArticles(1);
  cout << "After removing:" << endl;
  printArticles(testdeletearticle);
  cout<< endl;
  cout << "test deleteGroup by removing the group3:" << endl;
  mydb.deleteGroup(3);
  auto testdeletegroup = mydb.readGroups();
  printGroups(testdeletegroup);
  cout << endl;


}