#include "stdafx.h"
#include "token.h"
#include <iostream>
#include <string>
using namespace std;
int main(void)
{

    char* str;
    // string st        = "???????��????";
    path = "../in.txt";
    str              = openFile(path);
    scan(str);
    for (auto&& i : tokenArr)
    {
        cout << i.type << ' ' << string(i.start, i.end) << ' ' << i.val << ' ' << i.dval << endl;
    }

    getchar();
    return 0;
}