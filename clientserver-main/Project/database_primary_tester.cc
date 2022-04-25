#include "article.h"
#include "database.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

int main(){
    database test_database;

    //testa att skapa och skriva ut news groups
    test_database.create_group("hej");
    test_database.create_group("då");

    vector<string> test_groups = test_database.list_groups();


    for(auto e : test_groups){
        cout << "News group: " << endl;
        cout << e << endl;
    }

    //testa att ta bort news groups
    test_database.delete_group("då");

    vector<string> test_groups_deleted = test_database.list_groups();
    for(auto e : test_groups_deleted){
        cout << "News group: " << endl;
        cout << e << endl;
    }

    test_database.create_group("då");
    //testa att lägga till artiklar

    // article a("Hur man är king", "Gustav", "Man är skön");
    // article b("Hur man är en mes", "Tom", "Man är som jag");
    // article c("HUr man är en simp", "Matti", "Man är ihop med Ellen");


    return 0;
}