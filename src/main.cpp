#include "parse.h"
#include "stdafx.h"
#include "token.h"
#include <iostream>
#include <string>
using namespace std;
int main(void)
{

    const char* str;
    // string st        = "???????��????";
    path     = "../testCase/in1.txt";
    str      = openFile(path);
    buferror = str; // for parse error process
    scan(str);
    for (auto&& i : tokenArr)
    {
        if (i.type == "TK_INUM" or i.type == "TK_DNUM")
        {
            cout << i.type << ' ' << string(i.start, i.end) << ' ' << i.val << ' ' << i.dval << endl;
        }
        else
        {
            cout << i.type << ' ' << string(i.start, i.end) << endl;
        }
    }
    parse();
    getchar();
    return 0;
}