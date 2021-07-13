#include "codegen.h"
#include "optimize.h"
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
    cout << path << endl;
    cout << endl;
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

void usage()
{
    printf("Usage: cc <FilePath>\n");
    getchar();
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        path = "../testCase/in10.txt";
    }
    else if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        usage();
    }
    const char* str;
    str      = openFile(path);
    buferror = str; // for parse error process
    scan(str);
    outPutToken(true);
    parse();
    outPutParse(true);
    optimize();
    // getchar();
    cout << endl << endl << "CODE GEN " << endl;
    codeGen();
    return 0;
}