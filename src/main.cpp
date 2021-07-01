#include "stdafx.h"
#include "token.h"
#include<iostream>
#include<string>
using namespace std;
int main(void)
{
    char* str;
    string st = "Äã²ÂÄÄ´íÁË¡£";
    char* path = "in.txt";
    str = openFile(path);
    errorToken(str, str + 10, str+11, path, st);
    getchar();
    return 0;
}