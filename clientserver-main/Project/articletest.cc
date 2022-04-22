#include "article.h"

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){
    string t("test");
    string txt("hej");
    string auth("Kingen");
    article a(t, auth, txt);

    cout << a.getAuthor() << endl;
}

