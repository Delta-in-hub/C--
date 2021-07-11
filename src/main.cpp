#include "parse.h"
#include "stdafx.h"
#include "token.h"
#include <iostream>
#include <string>
using namespace std;

void outPutToken(bool flag = false)
{
    if (flag == false)
        return;
    cout << endl;
    for (auto&& i : tokenArr)
    {
        if (i.type == "TK_INUM" or i.type == "TK_DNUM")
        {
            cout << i.type << ' ' << string(i.start, i.end) << ' ' << i.val << ' ' << i.dval << ' ';
        }
        else
        {
            cout << i.type << ' ' << string(i.start, i.end) << ' ';
        }
    }
    cout << endl;
    cout << endl;
}

void outPutParse(bool flag = false)
{
    if (not flag)
        return;
    cout << endl;

    cout << "Global Variable" << endl;
    for (auto&& i : prog->gvars)
    {
        printf("%s sizeof %d VarType: %d \n", i->name.c_str(), i->ty->size, i->ty->ty);
    }
    cout << endl;

    cout << "Function" << endl;
    for (auto&& i : prog->funcs)
    {
        printf("%s returnType: %d \n", i->name.c_str(), i->returnType->ty);
        printf("It has %d params and function's address is %p\n", i->params->size(), i->compound);
    }
    cout << endl;
}

int main(void)
{

    const char* str;
    // string st        = "???????��????";
    path     = "../testCase/in4.txt";
    str      = openFile(path);
    buferror = str; // for parse error process
    scan(str);
    outPutToken(true);
    parse();
    outPutParse(true);
    getchar();
    return 0;
}