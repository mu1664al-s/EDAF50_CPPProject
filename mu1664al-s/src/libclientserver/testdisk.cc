#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include"dbdisk.h"
#include"dbinterface.h"
using namespace std;
void printArticle(const Article& article)
{
    cout << "ID:" << article.id << endl << "Name:" << article.name << endl << "Author:" <<article.author << endl << "Title:" << article.title << endl << "Text:" << article.text <<endl;
}
void printArticles(const vector<Article> articles)
{
    for(auto it = articles.begin();it!= articles.end(); it++)
    {
        printArticle(*it);
    }
}
int main()
{
    DBDisk mydb;

    //test initialize mydb by write Group and writeArticle. NB: check the database to see the change. New groupid.txt, article documents will be generated
    /*cout << "Test writeGroup and writeArticle" << endl;
    
    mydb.writeGroup("group1");
    mydb.writeGroup("group2");
    mydb.writeGroup("group3");
    Article article1{0,"article1", "vu", "title 1", "text\n1"};
    Article article2{0,"article2", "vu", "title 1", "text\n2"};
    Article article3{0,"article3", "vu", "title 1", "text\n3"};
    Article article4{0,"article4", "vu", "title 1", "text\n4"};
    Article article5{0,"article5", "vu", "title 1", "text\n5"};
    mydb.writeArticle(1,article1);
    mydb.writeArticle(2,article2);
    mydb.writeArticle(2,article3);
    mydb.writeArticle(3,article4);
    mydb.writeArticle(3,article5);*/
    //test ends here.
    
    //comment the first test and uncomment the second test;
    
    /*cout << "Test readArticle and readArticles" << endl;
    auto testreadArticle = mydb.readArticle(2,2);
    cout << "The article with id 2 in the group with id 2 (expected article3)" << endl;
    printArticle(testreadArticle);
    cout << "Print all the articles in group with id 3 (expected article 4 and article 5)" << endl;
    auto testreadArticles = mydb.readArticles(3);
    printArticles(testreadArticles);*/
    //test ends here.

    //comment all of the above and uncomment the last test; NB: CHECK THE newsgroup.txt and all the article document to see the change in the database. article document and group info will be removed.

    /*cout << "Test deleteArticle and deleteGroup" << endl;
    cout << "delete article  with id 1 in group1" << endl;
    mydb.deleteArticle(1,1);
    cout << "delete group with id 2" << endl;
    mydb.deleteGroup(2);*/
} 
